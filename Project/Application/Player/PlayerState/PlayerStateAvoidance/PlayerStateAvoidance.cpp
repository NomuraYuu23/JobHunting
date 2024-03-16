#include "PlayerStateAvoidance.h"
#include "../../Player.h"
#include "../../../../Engine/Math/Ease.h"

void PlayerStateAvoidance::Initialize()
{

	spped_ = 5.0f;

	frame_ = 30;

	frameCount_ = 0;

	avoidanceHeight_ = 5.0f;

	standHeight_ = 20.0f;

	playerStateNo_ = PlayerState::kPlayerStateAvoidance;

	playerMotionNo_ = kPlayerMotionAvoidance;

}

void PlayerStateAvoidance::Update()
{

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

	HeightUpdate();

	if (++frameCount_ >= frame_) {
		playerStateNo_ = PlayerState::kPlayerStateRoot;
	}
	player_->SetReceiveCommand(false);

}

void PlayerStateAvoidance::HeightUpdate()
{

	float height = 0.0f;

	if(frameCount_ < 10){
		float t = static_cast<float>(frameCount_) / 10.0f;
		height = Ease::Easing(Ease::EaseName::Lerp, standHeight_, avoidanceHeight_, t);
	}
	else if (frameCount_ < 20) {
		height = avoidanceHeight_;
	}
	else {
		float t = static_cast<float>(frameCount_ - 20) / 10.0f;
		height = Ease::Easing(Ease::EaseName::Lerp, avoidanceHeight_, standHeight_, t);
	}

	player_->SetHeight(height);

}
