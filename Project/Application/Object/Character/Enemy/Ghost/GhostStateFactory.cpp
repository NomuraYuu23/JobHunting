#include "GhostStateFactory.h"
#include <cassert>

#include "GhostStateRoot.h"
#include "GhostStateChase.h"
#include "GhostStateAttack.h"
#include "GhostStateDamage.h"
#include "GhostStateDead.h"
#include "GhostStateCadaver.h"

IGhostState* GhostStateFactory::CreateGhostState(uint32_t ghostStateName)
{
	// 次のステートを生成
	IGhostState* newGhostState = nullptr;

	switch (ghostStateName)
	{
	case kGhostStateRoot: // 通常
		newGhostState = new GhostStateRoot();
		break;
	case kGhostStateChase: // 追跡
		newGhostState = new GhostStateChase();
		break;
	case kGhostStateAttack: // 攻撃
		newGhostState = new GhostStateAttack();
		break;
	case kGhostStateDamage: // ダメージ
		newGhostState = new GhostStateDamage();
		break;
	case kGhostStateDead: // 死亡
		newGhostState = new GhostStateDead();
		break;
	case kGhostStateCadaver: // 死体
		newGhostState = new GhostStateCadaver();
		break;
	case kGhostStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newGhostState;

}
