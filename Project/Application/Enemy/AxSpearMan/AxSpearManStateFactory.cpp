#include "AxSpearManStateFactory.h"

#include "AxSpearManStateRoot.h"
#include "AxSpearManStateDashSwingDown.h"

IAxSpearManState* AxSpearManStateFactory::CreateAxSpearManState(uint32_t axSpearManStateName)
{
	// 次のステートを生成
	IAxSpearManState* newAxSpearManState = nullptr;

	switch (axSpearManStateName)
	{
	case kAxSpearManStateRoot: // 通常
		newAxSpearManState = new AxSpearManStateRoot();
		break;	
	case kAxSpearManStateDashSwingDown:// ダッシュ振り下ろし
		newAxSpearManState = new AxSpearManStateDashSwingDown();
		break;
	case kAxSpearManStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newAxSpearManState;
}
