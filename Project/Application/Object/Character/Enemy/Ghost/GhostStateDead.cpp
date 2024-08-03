#include "GhostStateDead.h"
#include "Ghost.h"

void GhostStateDead::Initialize()
{

	frame_ = 50;

	frameCount_ = 0;


	ghostStateNo_ = GhostState::kGhostStateDead;

	ghostMotionNo_ = kGhostMotionDead;

}

void GhostStateDead::Update()
{

	if (++frameCount_ >= frame_) {
		ghostStateNo_ = GhostState::kGhostStateCadaver;
	}
	ghost_->SetReceiveCommand(false);

}
