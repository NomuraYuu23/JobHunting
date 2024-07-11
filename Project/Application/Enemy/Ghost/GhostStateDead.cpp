#include "GhostStateDead.h"
#include "Ghost.h"

void GhostStateDead::Initialize()
{

	frame_ = 50;

	frameCount_ = 0;


	ghostStateNo_ = GhostState::kGhostStateDead;

	ghostMotionNo_ = kGhostStateDead;

}

void GhostStateDead::Update()
{

	if (++frameCount_ >= frame_) {
		ghost_->SetIsDead(true);
	}
	ghost_->SetReceiveCommand(false);

}
