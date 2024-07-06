#include "GhostStateDamage.h"
#include "Ghost.h"

void GhostStateDamage::Initialize()
{

	frameCount_ = 0;

	ghostMotionNo_ = kGhostMotionDamage;

	ghostStateNo_ = kGhostStateDamage;

}

void GhostStateDamage::Update()
{

	frameCount_++;

	if (frameCount_ >= frameEnd_) {
		ghostStateNo_ = kGhostStateRoot;
	}

	ghost_->SetReceiveCommand(false);

}
