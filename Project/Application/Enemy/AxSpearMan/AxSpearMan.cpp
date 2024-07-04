#include "AxSpearMan.h"

#include "AxSpearManStateFactory.h"

void AxSpearMan::Initialize(LevelData::MeshData* data)
{

	BaseEnemy::Initialize(data);

	height_ = 3.0f;
	worldTransform_.transform_.translate.y = height_;
	worldTransform_.UpdateMatrix();
	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

}

void AxSpearMan::Update()
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

void AxSpearMan::ImGuiDraw()
{
}

void AxSpearMan::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}

void AxSpearMan::StateInitialize()
{

	// ステート
	state_.reset(AxSpearManStateFactory::CreateAxSpearManState(AxSpearManState::kAxSpearManStateRoot)); // 最初のステート
	state_->Initialize();

	// ステート番号
	currentStateNo_ = AxSpearManState::kAxSpearManStateRoot; // 最初のステート
	prevStateNo_ = AxSpearManState::kAxSpearManStateRoot; // 最初のステート
	nextStateNo_ = AxSpearManState::kAxSpearManStateRoot; // 最初のステート
	state_->SetAxSpearMan(this);

}

void AxSpearMan::StateUpdate()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (receiveCommand_) {
		currentStateNo_ = nextStateNo_;
	}
	else {
		currentStateNo_ = state_->GetAxSpearManStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		state_.reset(AxSpearManStateFactory::CreateAxSpearManState(currentStateNo_));
		state_->Initialize();
	}

	// ステート更新
	state_->Update();

}

void AxSpearMan::CommandInitialize()
{

	// コマンド
	command_ = std::make_unique<AxSpearManCommand>();
	command_->Initialize(this);

	// コマンドを受け取るか
	receiveCommand_ = true;

}

void AxSpearMan::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = AxSpearManMotionIndex::kAxSpearManMotionIndexWait;

	// 前のモーション番号
	prevMotionNo_ = AxSpearManMotionIndex::kAxSpearManMotionIndexWait;

	// 待ちアニメーション
	animation_.StartAnimation(AxSpearManMotionIndex::kAxSpearManMotionIndexWait, true);

}

void AxSpearMan::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void AxSpearMan::AnimationUpdate()
{

	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = state_->GetAxSpearManMotionNo();

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

}
