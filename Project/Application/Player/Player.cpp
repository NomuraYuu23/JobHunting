#include "Player.h"
#include "../../Engine/Collider/Capsule/Capsule.h"
#include "../../Engine/2D/ImguiManager.h"
#include "../../Engine/3D/ModelDraw.h"
#include "../Enemy/Enemy.h"
#include "../TutorialEnemy/TutorialEnemy.h"
#include "../../Engine/Animation/LocalMatrixDraw.h"
#include "../../Engine/Math/DeltaTime.h"

#include "../Ground/Ground.h"
#include "../Block/Block.h"

void Player::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// コマンド
	playerCommand_ = PlayerCommand::GetInstance();
	playerCommand_->Initialize();

	// コマンドを受け取るか
	receiveCommand_ = true;

	// ステート
	StateInitialize();

	// パーツ
	PartInitialize();

	// hp
	hp_ = 3;
	initHp_ = 3;

	isDead_ = false;

	playerAttack_ = std::make_unique<PlayerAttack>();
	playerAttack_->Initialize(&worldTransform_);

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

}

void Player::Update()
{

	if (receiveCommand_) {
		nextStateNo_ = playerCommand_->Command();
	}

	// 前フレーム記録
	prePosition_ = worldTransform_.GetWorldPosition();

	// ステート
	StateUpdate();

	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	localMatrixManager_->Map();

	// 重力
	Gravity();

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

}

void Player::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void Player::ImGuiDraw()
{

	ImGui::Begin("Player");
	ImGui::Text("Position");
	ImGui::Text("X:%f, Y:%f, Z:%f", worldTransform_.worldMatrix_.m[3][0], worldTransform_.worldMatrix_.m[3][1], worldTransform_.worldMatrix_.m[3][2]);
	ImGui::Text("Direction");
	ImGui::Text("X:%f, Y:%f, Z:%f", worldTransform_.direction_.x, worldTransform_.direction_.y, worldTransform_.direction_.z);

	ImGui::Text("PlayerState");
	switch (currentStateNo_)
	{
	case kPlayerStateRoot: // 通常
		ImGui::Text("PlayerStateRoot");
		break;
	case kPlayerStateAvoidance: // 回避
		ImGui::Text("PlayerStateAvoidance");
		break;
	case kPlayerStateRecovery: // 回復
		ImGui::Text("PlayerStateRecovery");
		break;	
	case kPlayerStateGuard: // ガード
		ImGui::Text("PlayerStateGuard");
		break;
	case kPlayerStateAttack: // 攻撃
		ImGui::Text("PlayerStateAttack");
		break;
	}

	ImGui::End();

}

void Player::DebugDrawMap(DrawLine* drawLine)
{

	LocalMatrixDraw::DrawMap(
		localMatrixManager_.get(),
		&worldTransform_,
		drawLine);

}

void Player::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Enemy*>(colliderPartner)) {
		OnCollisionEnemy(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<TutorialEnemy*>(colliderPartner)) {
		OnCollisionTutorialEnemy(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<Ground*>(colliderPartner)) {
		OnCollisionGround(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<Block*>(colliderPartner)) {
		OnCollisionBlock(colliderPartner, collisionData);
	}

}

void Player::StateInitialize()
{

	// ステートファクトリー
	playerStateFactory_ = PlayerStateFactory::GetInstance();

	// ステート
	playerState_.reset(playerStateFactory_->CreatePlayerState(kPlayerStateRoot)); // 最初のステート
	playerState_->Initialize();

	// ステート番号
	currentStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	prevStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	nextStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	playerState_->SetPlayer(this); // プレイヤーセット

}

void Player::StateUpdate()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (receiveCommand_) {
		currentStateNo_ = nextStateNo_;
	}
	else {
		currentStateNo_ = playerState_->GetPlaryerStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		playerState_.reset(playerStateFactory_->CreatePlayerState(currentStateNo_));
		playerState_->Initialize();
	}

	// ステート更新
	playerState_->Update();

}

void Player::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = PlayerMotionIndex::kPlayerMotionWait;

	// 前のモーション番号
	prevMotionNo_ = PlayerMotionIndex::kPlayerMotionWait;

	// 待ちアニメーション
	animation_.StartAnimation(kPlayerMotionWait, true);

}

void Player::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ =  worldTransform_.GetWorldPosition();
	obb.center_.y += height_;
	

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void Player::AnimationUpdate()
{

	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = playerState_->GetPlaryerMotionNo();

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

}

void Player::OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	Enemy* enemy = std::get<Enemy*>(colliderPartner);

	// 位置
	Vector3 playerPosition = worldTransform_.GetWorldPosition();
	playerPosition.y = 0.0f;

	Vector3 enemyPosition = enemy->GetWorldTransformAdress()->GetWorldPosition();
	enemyPosition.y = 0.0f;

	// 向き
	Vector3 direction = Vector3::Normalize(Vector3::Subtract(playerPosition, enemyPosition));

	// 距離
	float distance = width_ + enemy->GetWidth();

	// 移動
	Vector3 move = Vector3::Multiply(distance, direction);

	worldTransform_.transform_.translate = Vector3::Add(enemyPosition, move);
	worldTransform_.UpdateMatrix();

}

void Player::OnCollisionTutorialEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	TutorialEnemy* enemy = std::get<TutorialEnemy*>(colliderPartner);

	// 位置
	Vector3 playerPosition = worldTransform_.GetWorldPosition();
	playerPosition.y = 0.0f;

	Vector3 enemyPosition = enemy->GetWorldTransformAdress()->GetWorldPosition();
	enemyPosition.y = 0.0f;

	// 向き
	Vector3 direction = Vector3::Normalize(Vector3::Subtract(playerPosition, enemyPosition));

	// 距離
	float distance = width_ + enemy->GetWidth();

	// 移動
	Vector3 move = Vector3::Multiply(distance, direction);

	worldTransform_.transform_.translate = Vector3::Add(enemyPosition, move);
	worldTransform_.UpdateMatrix();

}

void Player::OnCollisionGround(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	Ground* ground = std::get<Ground*>(colliderPartner);

	OBB obb = std::get<OBB>(*ground->GetCollider());

	worldTransform_.transform_.translate.y = ground->GetWorldTransformAdress()->GetWorldPosition().y + obb.size_.y;
	worldTransform_.UpdateMatrix();

}

void Player::OnCollisionBlock(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{


	//Block* block = std::get<Block*>(colliderPartner);

	//OBB blockObb = std::get<OBB>(*block->GetCollider());

	//Vector3 velocity = worldTransform_.GetWorldPosition() - prePosition_;

	//// 内積
	//float dot[3] = {
	//	Vector3::Dot(blockObb.otientatuons_[0] * blockObb.size_.x,velocity), // x
	//	Vector3::Dot(blockObb.otientatuons_[1] * blockObb.size_.y,velocity), // y
	//	Vector3::Dot(blockObb.otientatuons_[2] * blockObb.size_.z,velocity) // z
	//};

	//Vector3 move = { 0.0f,0.0f,0.0f };

	//if (std::fabsf(dot[0]) > std::fabsf(dot[1])) {
	//	if (std::fabsf(dot[0]) > std::fabsf(dot[2])) {
	//		move = blockObb.otientatuons_[0] * blockObb.size_.x;
	//		if (dot[0] > 0.0f) {
	//			move *= -1.0f;
	//		}
	//	}
	//	else {
	//		move = blockObb.otientatuons_[2] * blockObb.size_.z;
	//		if (dot[2] > 0.0f) {
	//			move *= -1.0f;
	//		}
	//	}
	//}
	//else {
	//	if (std::fabsf(dot[1]) > std::fabsf(dot[2])) {
	//		move = blockObb.otientatuons_[1] * blockObb.size_.y;
	//		if (dot[1] > 0.0f) {
	//			move *= -1.0f;
	//		}
	//	}
	//	else {
	//		move = blockObb.otientatuons_[2] * blockObb.size_.z;
	//		if (dot[2] > 0.0f) {
	//			move *= -1.0f;
	//		}
	//	}
	//}

	worldTransform_.transform_.translate = prePosition_;
	worldTransform_.UpdateMatrix();

}

void Player::Damage(uint32_t damage)
{

	hp_ -= damage;

	if (hp_ <= 0) {
		isDead_ = true;
	}

}

float Player::RatioHP()
{
	assert(initHp_ != 0);

	return static_cast<float>(hp_) / static_cast<float>(initHp_);
}

void Player::Gravity()
{

	worldTransform_.transform_.translate.y -= 9.8f * kDeltaTime_;

}
