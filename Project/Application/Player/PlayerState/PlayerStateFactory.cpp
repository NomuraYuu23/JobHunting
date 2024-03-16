#include "PlayerStateFactory.h"
#include "../PlayerState/PlayerStateRoot/PlayerStateRoot.h"
#include "../PlayerState/PlayerStateAvoidance/PlayerStateAvoidance.h"
#include "../PlayerState/PlayerStateRecovery/PlayerStateRecovery.h"
#include "../PlayerState/PlayerStateGuard/PlayerStateGuard.h"
#include "../PlayerState/PlayerStateAttack/PlayerStateAttack.h"
#include <cassert>

PlayerStateFactory* PlayerStateFactory::GetInstance()
{
	static PlayerStateFactory instance;
	return &instance;
}

IPlayerState* PlayerStateFactory::CreatePlayerState(uint32_t playerStateName)
{
	// 次のシーンを生成
	IPlayerState* newPlayerState = nullptr;

	switch (playerStateName)
	{
	case kPlayerStateRoot: // 通常
		newPlayerState = new PlayerStateRoot();
		break;
	case kPlayerStateAvoidance: // 回避
		newPlayerState = new PlayerStateAvoidance();
		break;
	case kPlayerStateRecovery: // 回復
		newPlayerState = new PlayerStateRecovery();
		break;
	case kPlayerStateGuard: // ガード
		newPlayerState = new PlayerStateGuard();
		break;
	case kPlayerStateAttack: // 攻撃
		newPlayerState = new PlayerStateAttack();
		break;
	case kPlayerStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newPlayerState;
}
