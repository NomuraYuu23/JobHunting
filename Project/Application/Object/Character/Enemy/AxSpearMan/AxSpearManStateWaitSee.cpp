#include "AxSpearManStateWaitSee.h"
#include "AxSpearMan.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Player/Player.h"

void AxSpearManStateWaitSee::Initialize()
{

	axSpearManMotionNo_ = kAxSpearManMotionWaitSee;

	inPhase_ = kPhaseStart;

	timer_ = 0.0f;

	axSpearManStateNo_ = AxSpearManState::kAxSpearManStateWaitSee;

	distance_ = 0.0f;

}

void AxSpearManStateWaitSee::Update()
{

	switch (inPhase_)
	{
	case AxSpearManStateWaitSee::kPhaseStart:
		StartUpdate();
		break;
	case AxSpearManStateWaitSee::kPhaseMove:
		MoveUpdate();
		break;
	case AxSpearManStateWaitSee::KPhaseEnd:
		EndUpdate();
		break;
	case AxSpearManStateWaitSee::kPhaseOfCount:
	default:
		break;
	}

	axSpearMan_->SetReceiveCommand(false);

}

void AxSpearManStateWaitSee::StartUpdate()
{

	timer_ += static_cast<double>(kDeltaTime_);

	if (timer_ >= kAnimationTimeWhenMoving) {
		inPhase_ = kPhaseMove;
	}

}

void AxSpearManStateWaitSee::MoveUpdate()
{
	
	// タイマー固定
	axSpearMan_->GetAnimationAddress()->GetAnimationDatas()->at(kAxSpearManMotionWaitSee).timer = kAnimationTimeWhenMoving - static_cast<double>(kDeltaTime_);

	// 移動
	Move();

	// 終了条件
	if (distance_ <= kEndDistance) {
		inPhase_ = KPhaseEnd;
	}

}

void AxSpearManStateWaitSee::EndUpdate()
{

	timer_ += static_cast<double>(kDeltaTime_);

	if (timer_ >= kEndTime) {
		inPhase_ = kPhaseMove;
		axSpearManStateNo_ = AxSpearManState::kAxSpearManStateRoot;
	}

}

void AxSpearManStateWaitSee::Move()
{

	WorldTransform* worldTransform = axSpearMan_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	Player* player = axSpearMan_->GetPlayer();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	WorldTransform* playerWorldTransform = player->GetWorldTransformAdress();

	distance_ = Vector3::Length(Vector3::Subtract(playerWorldTransform->GetWorldPosition(), worldTransform->GetWorldPosition()));

	// 向き
	Vector3 direction = worldTransform->direction_;
	direction = Vector3::Normalize(Vector3::Subtract(playerWorldTransform->GetWorldPosition(), worldTransform->GetWorldPosition()));

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };

	// 移動量に速さを反映
	move = Vector3::Multiply(0.1f, Vector3::Normalize(move));

	Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f, 1.0f }, direction);

	// 移動ベクトルをカメラの角度だけ回転する
	Vector3 targetDirection = Matrix4x4::TransformNormal(move, rotateMatrix);
	move = Matrix4x4::TransformNormal(move, Matrix4x4::Multiply(rotateMatrix, Matrix4x4::MakeRotateYMatrix(1.57f)));

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	targetDirection.y = 0.0f;
	targetDirection = Vector3::Normalize(targetDirection);

	// 角度補間
	worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection, 0.1f);

	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

}
