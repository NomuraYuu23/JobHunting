#pragma once
#include "AxSpearMan.h"

class AxSpearManStateFactory
{

public: // メンバ関数

	// ステート生成
	static IAxSpearManState* CreateAxSpearManState(uint32_t axSpearManStateName);

};

