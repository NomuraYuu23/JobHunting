#include "Player.h"
#include "../../Engine/Collider/Capsule/Capsule.h"
#include "../../Engine/2D/ImguiManager.h"
//#include "../Enemy/Enemy.h"

void Player::Initialize(Model* model, Model* weaponModel)
{

	// モデル
	model_ = model;

	// マテリアル
	material_.reset(Material::Create());
	material_->SetEnableLighting(BlinnPhongReflection);

	// ワールドトランスフォーム
	worldTransform_.Initialize(model_->GetRootNode());

	// コマンド
	playerCommand_ = PlayerCommand::GetInstance();
	playerCommand_->Initialize();

	// コマンドを受け取るか
	receiveCommand_ = true;

	// ステート
	StateInitialize();

	// パーツ
	PartInitialize();

	// コライダー
	ColliderInitialize();

	// 武器モデル
	weaponModel_ = weaponModel;
	weaponWorldTransfrom_.Initialize(weaponModel_->GetRootNode());
	weaponWorldTransfrom_.parent_ = &worldTransform_;
	weaponWorldTransfrom_.transform_.rotate.x = 1.57f;
	weaponWorldTransfrom_.transform_.rotate.z = 1.57f;
	weaponWorldTransfrom_.transform_.translate.y = -2.0f;
	weaponWorldTransfrom_.UpdateMatrix();

	// hp
	hp_ = 3;
	initHp_ = 3;

	isDead_ = false;

}

void Player::Update()
{

	if (receiveCommand_) {
		nextStateNo_ = playerCommand_->Command();
	}

	// ステート
	StateUpdate();

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 武器
	weaponWorldTransfrom_.UpdateMatrix();

}

void Player::Draw(BaseCamera& camera)
{

	model_->Draw(worldTransform_, camera, material_.get());

	weaponModel_->Draw(weaponWorldTransfrom_, camera, material_.get());

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

void Player::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	//if (std::holds_alternative<Enemy*>(colliderPartner)) {
	//	OnCollisionEnemy(colliderPartner, collisionData);
	//}

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
	currentMotionNo_ = PlayerMotionIndex::kPlayerMotionStand;

	// 前のモーション番号
	prevMotionNo_ = PlayerMotionIndex::kPlayerMotionStand;

	// アニメーションカウント
	animationCount_ = 0u;

	// アニメーションカウント上限
	animationCountLimit_ = 0u;

}

void Player::ColliderInitialize()
{

	// コライダー
	collider_ = std::make_unique<Capsule>();
	collider_->SetCollisionAttribute(collisionAttribute_);
	collider_->SetCollisionMask(collisionMask_);
	collider_->radius_ = 1.0f;

		//計算用
	Segment segment = {
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
	};
	Vector3 diff = { 0.0f,0.0f,0.0f };

}

void Player::ColliderUpdate()
{

	//計算用
	Segment segment = {
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
	};
	Vector3 diff = { 0.0f,0.0f,0.0f };


}

void Player::OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	//Enemy* enemy = std::get<Enemy*>(colliderPartner);

	//// 位置
	//Vector3 playerPosition = worldTransform_.GetWorldPosition();
	//playerPosition.y = 0.0f;

	//Vector3 enemyPosition = enemy->GetWorldTransformAdress()->GetWorldPosition();
	//enemyPosition.y = 0.0f;

	//// 向き
	//Vector3 direction = Vector3Calc::Normalize(Vector3Calc::Subtract(playerPosition, enemyPosition));

	//// 距離
	//float distance = width_ + enemy->GetWidth();

	//// 移動
	//Vector3 move = Vector3Calc::Multiply(distance, direction);

	//worldTransform_.transform_.translate = Vector3Calc::Add(enemyPosition, move);
	//worldTransform_.transform_.translate.y = height_;
	//worldTransform_.UpdateMatrix();

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
