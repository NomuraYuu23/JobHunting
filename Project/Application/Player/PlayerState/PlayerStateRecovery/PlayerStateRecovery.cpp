#include "PlayerStateRecovery.h"
#include "../../Player.h"
#include "../../../../Engine/Math/Ease.h"

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

	BaseCamera* camera = player_->GetCamera();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	// 移動量に速さを反映
	move = Vector3::Multiply(spped_, Vector3::Normalize(move));

	// カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(camera->GetRotate());

	// 移動ベクトルをカメラの角度だけ回転する
	move = Matrix4x4::TransformNormal(move, rotateMatrix);

	//　親がいれば
	if (worldTransform->parent_) {
		rotateMatrix = Matrix4x4::Inverse(worldTransform->parent_->rotateMatrix_);
		move = Matrix4x4::TransformNormal(move, rotateMatrix);
	}

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	// あたん
	targetDirection_.x = Vector3::Normalize(move).x;
	targetDirection_.z = Vector3::Normalize(move).z;

	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

}

void PlayerStateRecovery::Recovery()
{
}
