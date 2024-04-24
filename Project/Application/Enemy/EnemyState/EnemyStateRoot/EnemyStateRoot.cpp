#include "EnemyStateRoot.h"
#include "../../Enemy.h"

void EnemyStateRoot::Initialize()
{

	enemyMotionNo_ = kEnemyMotionStand;

}

void EnemyStateRoot::Update()
{

	enemyMotionNo_ = kEnemyMotionStand;

	enemy_->SetReceiveCommand(true);

}
