#include "Ghost.h"
#include "GhostStateFactory.h"

void Ghost::Initialize(LevelData::MeshData* data)
{

	BaseEnemy::Initialize(data);

	height_ = 3.0f;
	worldTransform_.transform_.translate.y = height_;
	worldTransform_.UpdateMatrix();
	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

}

void Ghost::Update()
{

	if (receiveCommand_) {
		nextStateNo_ = command_->Command();
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

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

}

void Ghost::ImGuiDraw()
{
}

void Ghost::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
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
	if (receiveCommand_) {
		currentStateNo_ = nextStateNo_;
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
	command_->Initialize(this, player_);

	// コマンドを受け取るか
	receiveCommand_ = true;

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
