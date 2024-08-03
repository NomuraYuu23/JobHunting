#include "PlayerStateAvoidance.h"
#include "../../Player.h"
#include "../../../../../../Engine/Math/Ease.h"

void PlayerStateAvoidance::Initialize()
{

	spped_ = 0.5f;

	frame_ = 20;

	frameCount_ = 0;


	playerStateNo_ = PlayerState::kPlayerStateAvoidance;

	playerMotionNo_ = kPlayerMotionAvoidance;

}

void PlayerStateAvoidance::Update()
{

	if (frameCount_ > moveStartFrame_) {

		Vector3 velocity = { 0.0f, 0.0f, 0.0f };

		WorldTransform* worldTransform = player_->GetWorldTransformAdress();

		Vector3 move = { 0.0f,0.0f,1.0f };

		// 移動量に速さを反映
		move = Vector3::Multiply(spped_, Vector3::Normalize(move));

		// 移動ベクトルをカメラの角度だけ回転する
		move = Matrix4x4::TransformNormal(move, worldTransform->rotateMatrix_);

		// 移動
		velocity.x = move.x;
		velocity.z = move.z;

		worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

	}

	if (++frameCount_ >= frame_) {
		playerStateNo_ = PlayerState::kPlayerStateRoot;
	}
	player_->SetReceiveCommand(false);

}

