#include "Player.h"
#include "../../../Engine/Collider/Capsule/Capsule.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../../Engine/Animation/LocalMatrixDraw.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Physics/Gravity.h"
#include "../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../Enemy/BaseEnemy.h"
#include <random>

void Player::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	//衝突属性(自分)
	collisionAttribute_ = 0x00000001;

	// 衝突マスク(相手)
	collisionMask_ = 0xfffffffe;

	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
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

	attack_ = std::make_unique<PlayerAttack>();
	attack_->Initialize(&worldTransform_);

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	dxCommon_ = DirectXCommon::GetInstance();

	// フィールドパーティクル
	fieldSparksParticle_ = std::make_unique<FieldSparksParticle>();
	fieldSparksParticle_->Initialize(
		dxCommon_->GetDevice(),
		dxCommon_->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get());

	fieldSparksEmitter_.count = 5;
	fieldSparksEmitter_.frequency = 0.5f;
	fieldSparksEmitter_.frequencyTime = 0.0f;
	fieldSparksEmitter_.translate = worldTransform_.transform_.translate;
	fieldSparksEmitter_.radius = 10.0f;
	fieldSparksEmitter_.emit = 0;

	fieldSparksParticle_->SetEmitter(fieldSparksEmitter_);

	CloakInitialize();

}

void Player::Update()
{

	MeshObject::Update();

	if (receiveCommand_) {
		nextStateNo_ = playerCommand_->Command();
	}

	// ステート
	StateUpdate();

	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	localMatrixManager_->Map();

	// 重力
	worldTransform_.transform_.translate += Gravity::Execute();

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

	attack_->ParticleUpdate();

	// フィールドパーティクル
	fieldSparksParticle_->Update();
	fieldSparksEmitter_.translate = worldTransform_.GetWorldPosition();
	fieldSparksParticle_->SetEmitter(fieldSparksEmitter_,false);

	CloakUpdate();

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


	cloak_->Draw(dxCommon_->GetCommadList(), &camera);

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

	if (std::holds_alternative<BaseEnemy*>(colliderPartner)) {
		OnCollisionEnemy(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<BaseObstacle*>(colliderPartner)) {
		OnCollisionObstacle(colliderPartner, collisionData);
	}

}

void Player::CollisionListRegister(CollisionManager* collisionManager)
{

	MeshObject::CollisionListRegister(collisionManager);

	attack_->CollisionListRegister(collisionManager);


}

void Player::CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);

	attack_->CollisionListRegister(collisionManager, colliderDebugDraw);

}

void Player::ParticleDraw(BaseCamera& camera)
{

	assert(commandList_);

	attack_->ParticleDraw(commandList_, camera);

	fieldSparksParticle_->Draw(commandList_, camera);

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
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);
	

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

	BaseEnemy* enemy = std::get<BaseEnemy*>(colliderPartner);

	Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*collider_), &std::get<OBB>(*enemy->GetCollider()));

	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix();

}

void Player::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	BaseObstacle* obstacle = std::get<BaseObstacle*>(colliderPartner);

	Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*collider_), &std::get<OBB>(*obstacle->GetCollider()));

	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix();

}

void Player::SetCloakPosition()
{

	// 
	for (uint32_t y = 0; y <= static_cast<uint32_t>(cloakDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(cloakDiv_.x); ++x) {
			cloak_->SetWeight(y, x, true);
			cloak_->SetPosition(y, x, cloakRightPos_);
		}
	}

}

void Player::CloakNodeFollowing()
{

	// 位置行列
	Vector3 pos = {
	parentRightNodeData_->matrix.m[3][0],
	parentRightNodeData_->matrix.m[3][1],
	parentRightNodeData_->matrix.m[3][2] };

	pos += cloakRightLocalPos_;
	pos = Matrix4x4::Transform(pos, worldTransform_.parentMatrix_);

	// 回転行列
	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(parentRightNodeData_->offsetMatrix, parentRightNodeData_->matrix);
	rotateMatrix.m[3][0] = 0.0f;
	rotateMatrix.m[3][1] = 0.0f;
	rotateMatrix.m[3][2] = 0.0f;

	Matrix4x4 parentRotateMatrix = worldTransform_.parentMatrix_;

	parentRotateMatrix.m[3][0] = 0.0f;
	parentRotateMatrix.m[3][1] = 0.0f;
	parentRotateMatrix.m[3][2] = 0.0f;

	rotateMatrix = Matrix4x4::Multiply(rotateMatrix, parentRotateMatrix);

	Matrix4x4 newMatrix = Matrix4x4::Multiply(rotateMatrix, Matrix4x4::MakeTranslateMatrix(pos));

	cloakRightPos_ = {
	newMatrix.m[3][0],
	newMatrix.m[3][1],
	newMatrix.m[3][2] };

	// 位置行列
	pos = {
	parentLeftNodeData_->matrix.m[3][0],
	parentLeftNodeData_->matrix.m[3][1],
	parentLeftNodeData_->matrix.m[3][2] };

	pos += cloakLeftLocalPos_;
	pos = Matrix4x4::Transform(pos, worldTransform_.parentMatrix_);

	// 回転行列
	rotateMatrix = Matrix4x4::Multiply(parentLeftNodeData_->offsetMatrix, parentLeftNodeData_->matrix);
	rotateMatrix.m[3][0] = 0.0f;
	rotateMatrix.m[3][1] = 0.0f;
	rotateMatrix.m[3][2] = 0.0f;

	parentRotateMatrix = worldTransform_.parentMatrix_;

	parentRotateMatrix.m[3][0] = 0.0f;
	parentRotateMatrix.m[3][1] = 0.0f;
	parentRotateMatrix.m[3][2] = 0.0f;

	rotateMatrix = Matrix4x4::Multiply(rotateMatrix, parentRotateMatrix);

	newMatrix = Matrix4x4::Multiply(rotateMatrix, Matrix4x4::MakeTranslateMatrix(pos));

	cloakLeftPos_ = {
	newMatrix.m[3][0],
	newMatrix.m[3][1],
	newMatrix.m[3][2] };

}

void Player::CloakInitialize()
{

	cloakDiv_ = Vector2{ 15.0f, 15.0f };

	cloakScale_ = Vector2{ 2.0f, 0.6f };

	cloak_ = std::make_unique<ClothGPU>();
	cloak_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), cloakScale_, cloakDiv_, "Resources/Sprite/Cloth/PlayerCloth.png");

	cloakIsPosSet_ = true;

	std::vector<std::string> names = localMatrixManager_->GetNodeNames();

	const std::string& parentRightName = "mixamorig:RightArm";
	const std::string& parentLeftName = "mixamorig:LeftArm";

	for (uint32_t i = 0; i < names.size(); ++i) {
		if (parentRightName == names[i]) {
			parentRightNodeData_ = &localMatrixManager_->GetNodeDatasAddress()->at(i);
		}
		else if (parentLeftName == names[i]) {
			parentLeftNodeData_ = &localMatrixManager_->GetNodeDatasAddress()->at(i);
		}
	}

	cloak_->SetStiffness(200.0f);
	cloak_->SetSpeedResistance(0.9f);
	cloak_->SetStructuralShrink(50.0f);
	cloak_->SetStructuralStretch(50.0f);
	cloak_->SetShearShrink(50.0f);
	cloak_->SetShearStretch(50.0f);
	cloak_->SetBendingShrink(50.0f);
	cloak_->SetBendingStretch(50.0f);


	cloakRightLocalPos_ = {0.15f, 0.3f, 0.0f};
	cloakLeftLocalPos_ = { -0.15f, 0.3f, 0.0f };

}

void Player::CloakUpdate()
{

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distribution(0.0f, 50.0f);

	Vector3 dir = worldTransform_.direction_ * -1.0f;

	Vector3 wind = { 0.0f, 0.0f, dir.z * distribution(randomEngine) };

	cloak_->SetWind(wind);

	cloak_->Update(dxCommon_->GetCommadList());

	if (cloakIsPosSet_) {
		SetCloakPosition();
		cloakIsPosSet_ = false;
	}

	CloakNodeFollowing();

	// 固定する
	cloak_->SetWeight(0, 0, false);
	cloak_->SetPosition(0, 0, cloakRightPos_);
	cloak_->SetWeight(1, 0, false);
	cloak_->SetPosition(1, 0, cloakRightPos_);
	cloak_->SetWeight(2, 0, false);
	cloak_->SetPosition(2, 0, cloakRightPos_);

	cloak_->SetWeight(static_cast<uint32_t>(cloakDiv_.y), 0, false);
	cloak_->SetPosition(static_cast<uint32_t>(cloakDiv_.y), 0, cloakLeftPos_);
	cloak_->SetWeight(static_cast<uint32_t>(cloakDiv_.y) - 1, 0, false);
	cloak_->SetPosition(static_cast<uint32_t>(cloakDiv_.y) - 1, 0, cloakLeftPos_);
	cloak_->SetWeight(static_cast<uint32_t>(cloakDiv_.y) - 2, 0, false);
	cloak_->SetPosition(static_cast<uint32_t>(cloakDiv_.y) - 2, 0, cloakLeftPos_);

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
