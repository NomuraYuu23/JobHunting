#include "AxSpearManStateRoot.h"
#include "AxSpearMan.h"

void AxSpearManStateRoot::Initialize()
{

	axSpearManMotionNo_ = kAxSpearManStateRoot;

}

void AxSpearManStateRoot::Update()
{

	axSpearMan_->SetReceiveCommand(true);

}
