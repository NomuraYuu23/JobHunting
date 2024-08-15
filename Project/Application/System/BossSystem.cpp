#include "BossSystem.h"

void BossSystem::Initialize(BaseObjectManager* objectManager)
{

	player_ = static_cast<Player*>(objectManager->GetObjectPointer(kPlayerName_));
	
	if (!player_) {
		assert(0);
	}

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

	Vector3 playerPostion = player_->GetWorldTransformAdress()->GetWorldPosition();

	Vector3 bossPostion = boss_->GetWorldTransformAdress()->GetWorldPosition();

	float distance = Vector3::Length(playerPostion - bossPostion);

	if (kBattleDistance_ > distance) {
		isBossBattle_ = true;
	}

}
