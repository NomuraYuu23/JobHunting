#include "EnemyStateFactory.h"
#include <cassert>
#include "EnemyStateRoot/EnemyStateRoot.h"
#include "EnemyStateDashSwingDown/EnemyStateDashSwingDown.h"
#include "EnemyStateMowingDown/EnemyStateMowingDown.h"

EnemyStateFactory* EnemyStateFactory::GetInstance()
{
	static EnemyStateFactory instance;
	return &instance;
}

IEnemyState* EnemyStateFactory::CreateEnemyState(uint32_t enemyStateName)
{
	// 次のシーンを生成
	IEnemyState* newEnemyState = nullptr;

	switch (enemyStateName)
	{
	case kEnemyStateRoot: // 通常
		newEnemyState = new EnemyStateRoot();
		break;
	case kEnemyStateDashSwingDown: // ダッシュ振り下ろし
		newEnemyState = new EnemyStateDashSwingDown();
		break;
	case kEnemyStateMowingDown: // ダッシュ振り下ろし
		newEnemyState = new EnemyStateMowingDown();
		break;
	case kEnemyStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newEnemyState;
}
