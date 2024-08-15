#include "BossSystem.h"
#include "../Object/Character/Enemy/AxSpearMan/IAxSpearManState.h"

void BossSystem::Initialize(BaseObjectManager* objectManager)
{

	boss_ = static_cast<BaseEnemy*>(objectManager->GetObjectPointer(kBossName_));

	if (!boss_) {
		assert(0);
	}

	isBossBattle_ = false;

}

void BossSystem::Update()
{

	if (isBossBattle_) {
		return;
	}

	if (boss_->GetCurrentStateNo() != kAxSpearManStateWaitingForBattle) {
		isBossBattle_ = true;
	}

}
