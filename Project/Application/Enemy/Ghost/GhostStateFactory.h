#pragma once
#include "IGhostState.h"


class GhostStateFactory
{

public: // メンバ関数

	// ステート生成
	static IGhostState* CreateGhostState(uint32_t ghostStateName);

};

