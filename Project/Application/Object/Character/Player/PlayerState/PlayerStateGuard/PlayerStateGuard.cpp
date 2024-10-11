#include "PlayerStateGuard.h"
#include "../../Player.h"
#include "../../../../../../Engine/Math/Ease.h"

void PlayerStateGuard::Initialize()
{

	spped_ = 2.0f;

	frame_ = 20;

	frameCount_ = 0;

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	playerStateNo_ = PlayerState::kPlayerStateGuard;

}

void PlayerStateGuard::Update()
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
			playerMotionNo_ = kPlayerMotionGuardWalk;
		}
		else {
			playerMotionNo_ = kPlayerMotionGuard;
		}

		// 角度補間
		worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);
	}

	player_->SetReceiveCommand(true);

}

void PlayerStateGuard::Move(Vector3& move, WorldTransform* worldTransform)
{
}
