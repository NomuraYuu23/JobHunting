#include "Ghost.h"
#include "GhostStateFactory.h"

void Ghost::Initialize(LevelData::MeshData* data)
{

	BaseEnemy::Initialize(data);

	height_ = 3.0f;
	worldTransform_.transform_.translate.y = height_;
	worldTransform_.UpdateMatrix();
	prePosition_ = worldTransform_.GetWorldPosition();

	isDead_ = false;

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	HPInit(3);

}

void Ghost::Update()
{

	BaseEnemy::Update();

	if (receiveCommand_) {
		nextStateNo_ = command_->Command();
	}

	// ステート
	StateUpdate();

	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	localMatrixManager_->Map();

	// ワールドトランスフォーム更新
	if (currentStateNo_ != kGhostMotionCadaver) {
		worldTransform_.UpdateMatrix();
	}



	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Ghost::ImGuiDraw()
{
}

void Ghost::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<BaseObstacle*>(colliderPartner)) {
		OnCollisionObstacle(colliderPartner, collisionData);
	}

}

void Ghost::StateInitialize()
{

	// ステート
	state_.reset(GhostStateFactory::CreateGhostState(GhostState::kGhostStateRoot)); // 最初のステート
	state_->Initialize();

	// ステート番号
	currentStateNo_ = GhostState::kGhostStateRoot; // 最初のステート
	prevStateNo_ = GhostState::kGhostStateRoot; // 最初のステート
	nextStateNo_ = GhostState::kGhostStateRoot; // 最初のステート
	state_->SetGhost(this);

}

void Ghost::StateUpdate()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (receiveCommand_ || interruptCommand_) {
		currentStateNo_ = nextStateNo_;
		interruptCommand_ = false;
	}
	else {
		currentStateNo_ = state_->GetGhostStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		state_.reset(GhostStateFactory::CreateGhostState(currentStateNo_));
		state_->Initialize();
	}

	// ステート更新
	state_->Update();

}

void Ghost::CommandInitialize()
{

	// コマンド
	command_ = std::make_unique<GhostCommand>();
	command_->Initialize(this);

	// コマンドを受け取るか
	receiveCommand_ = true;

	// 割り込みコマンドがあるか
	interruptCommand_ = false;

}

void Ghost::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = GhostMotionIndex::kGhostMotionWait;

	// 前のモーション番号
	prevMotionNo_ = GhostMotionIndex::kGhostMotionWait;

	// 待ちアニメーション
	animation_.StartAnimation(kGhostMotionWait, true);

}

void Ghost::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void Ghost::AnimationUpdate()
{

	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = state_->GetGhostMotionNo();

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

}

void Ghost::Damage(uint32_t damage)
{

	if (currentStateNo_ == kGhostStateDead || currentStateNo_ == kGhostStateCadaver) {
		return;
	}

	BaseEnemy::Damage(damage);

	if (hp_ <= 0) {
		nextStateNo_ = kGhostStateDead;
		interruptCommand_ = true;
	}
	else {
		nextStateNo_ = kGhostStateDamage;
		interruptCommand_ = true;
	}

}
