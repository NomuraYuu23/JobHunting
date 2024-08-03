#include "GhostStateRoot.h"
#include "Ghost.h"

void GhostStateRoot::Initialize()
{

	ghostMotionNo_ = kGhostMotionWait;

}

void GhostStateRoot::Update()
{

	ghost_->SetReceiveCommand(true);

}
