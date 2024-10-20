#include "PlayerStateDamage.h"
#include "../../Player.h"

void PlayerStateDamage::Initialize()
{

	frameCount_ = 0;

	playerMotionNo_ = kPlayerMotionDamage;

	playerStateNo_ = kPlayerStateDamage;

}

void PlayerStateDamage::Update()
{

	frameCount_++;

	if (frameCount_ >= frameEnd_) {
		playerStateNo_ = kPlayerStateRoot;
	}

	player_->SetReceiveCommand(false);

}
