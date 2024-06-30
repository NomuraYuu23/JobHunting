#include "GhostStateFactory.h"
#include <cassert>

IGhostState* GhostStateFactory::CreateGhostState(uint32_t ghostStateName)
{
	// 次のステートを生成
	IGhostState* newGhostState = nullptr;

	switch (ghostStateName)
	{
	case kGhostStateRoot: // 通常
		//newGhostState = new ();
		break;
	case kGhostStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newGhostState;

}
