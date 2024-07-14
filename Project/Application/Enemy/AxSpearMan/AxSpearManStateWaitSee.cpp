#include "AxSpearManStateWaitSee.h"
#include "AxSpearMan.h"

void AxSpearManStateWaitSee::Initialize()
{

	axSpearManMotionNo_ = kAxSpearManMotionWaitSee;

	inPhase_ = kPhaseStart;

	timer_ = 0.0f;

	axSpearManStateNo_ = AxSpearManState::kAxSpearManStateWaitSee;

}

void AxSpearManStateWaitSee::Update()
{

	switch (inPhase_)
	{
	case AxSpearManStateWaitSee::kPhaseStart:
		StartUpdate();
		break;
	case AxSpearManStateWaitSee::kPhaseMove:
		MoveUpdate();
		break;
	case AxSpearManStateWaitSee::KPhaseEnd:
		EndUpdate();
		break;
	case AxSpearManStateWaitSee::kPhaseOfCount:
	default:
		break;
	}

	axSpearMan_->SetReceiveCommand(false);

}

void AxSpearManStateWaitSee::StartUpdate()
{

	timer_ += static_cast<double>(kDeltaTime_);

	if (timer_ >= kAnimationTimeWhenMoving) {
		inPhase_ = kPhaseMove;
	}

}

void AxSpearManStateWaitSee::MoveUpdate()
{
	
	// タイマー固定
	axSpearMan_->GetAnimationAddress()->GetAnimationDatas()->at(kAxSpearManMotionWaitSee).timer = kAnimationTimeWhenMoving - static_cast<double>(kDeltaTime_);

	// 移動

	// 終了条件

}

void AxSpearManStateWaitSee::EndUpdate()
{

	timer_ += static_cast<double>(kDeltaTime_);

	if (timer_ >= kEndTime) {
		inPhase_ = kPhaseMove;
		axSpearManStateNo_ = AxSpearManState::kAxSpearManStateRoot;
	}

}
