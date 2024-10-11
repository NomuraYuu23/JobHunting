#include "PlayerStateRecovery.h"
#include "../../Player.h"
#include "../../../../../../Engine/Math/Ease.h"

void PlayerStateRecovery::Initialize()
{

	spped_ = 2.0f;

	frame_ = 20;

	frameCount_ = 0;

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	recoverySpeed_ = 10;

	playerStateNo_ = PlayerState::kPlayerStateRecovery;

	playerMotionNo_ = kPlayerMotionRecovery;

}

void PlayerStateRecovery::Update()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	//移動
	if (input_->GetJoystickConnected()) {

		const float kThresholdWalk = 0.5f;

		// 移動量
		Vector3 move = { input_->GetLeftAnalogstick().x, 0.0f, -input_->GetLeftAnalogstick().y };
		if (Vector3::Length(move) > kThresholdWalk) {
			Move(move, worldTransform);
		}

		// 角度補間
		worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);
	}

	// 回復
	Recovery();

	if (++frameCount_ >= frame_) {
		playerStateNo_ = PlayerState::kPlayerStateRoot;
	}
	player_->SetReceiveCommand(false);

}

void PlayerStateRecovery::Move(Vector3& move, WorldTransform* worldTransform)
{
}

void PlayerStateRecovery::Recovery()
{
}
