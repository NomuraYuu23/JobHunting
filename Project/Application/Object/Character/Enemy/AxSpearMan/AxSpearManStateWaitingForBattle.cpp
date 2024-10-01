#include "AxSpearManStateWaitingForBattle.h"
#include "AxSpearMan.h"
#include "../../Player/Player.h"

void AxSpearManStateWaitingForBattle::Initialize()
{

	axSpearManMotionNo_ = kAxSpearManStateRoot;
	axSpearManStateNo_ = kAxSpearManStateWaitingForBattle;

	// 確認までのフレーム
	checkFrame_ = 0;
	checkFrameMax_ = 10;

}

void AxSpearManStateWaitingForBattle::Update()
{

	checkFrame_++;
	if (checkFrame_ >= checkFrameMax_) {
		checkFrame_ = checkFrameMax_;
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
	else {
		axSpearMan_->SetReceiveCommand(false);
	}
}
