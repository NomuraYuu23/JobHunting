#include "BossSystem.h"
#include "../Object/Character/Enemy/AxSpearMan/IAxSpearManState.h"

void BossSystem::Initialize(BaseObjectManager* objectManager)
{

	boss_ = static_cast<BaseEnemy*>(objectManager->GetObjectPointer(kBossName_));

	if (!boss_) {
		assert(0);
	}

	isBossBattle_ = false;

	// 確認までのフレーム
	checkFrame_ = 0;
	checkFrameMax_ = 10;

}

void BossSystem::Update()
{

	if (isBossBattle_) {
		return;
	}

	checkFrame_++;
	if (checkFrame_ >= checkFrameMax_) {
		checkFrame_ = checkFrameMax_;
	}
	else {
		return;
	}

	if (boss_->GetCurrentStateNo() != kAxSpearManStateWaitingForBattle) {
		isBossBattle_ = true;
	}

}
