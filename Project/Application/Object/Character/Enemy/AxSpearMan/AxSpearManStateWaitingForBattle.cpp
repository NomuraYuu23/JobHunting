#include "AxSpearManStateWaitingForBattle.h"
#include "AxSpearMan.h"
#include "../../Player/Player.h"

void AxSpearManStateWaitingForBattle::Initialize()
{

	axSpearManMotionNo_ = kAxSpearManStateRoot;

}

void AxSpearManStateWaitingForBattle::Update()
{

	Vector3 playerPos = axSpearMan_->GetPlayer()->GetWorldTransformAdress()->GetWorldPosition();

	Vector3 myPos = axSpearMan_->GetWorldTransformAdress()->GetWorldPosition();

	float distance = Vector3::Length(playerPos - myPos);

	if (kBattleDistance_ > distance) {
		axSpearMan_->SetReceiveCommand(true);
	}
	else {
		axSpearMan_->SetReceiveCommand(false);
	}

}
