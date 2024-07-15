#include "AxSpearManStateFactory.h"

#include "AxSpearManStateRoot.h"
#include "AxSpearManStateDashSwingDown.h"
#include "AxSpearManStateWaitSee.h"
#include "AxSpearManStateShakeOff.h"

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
	case kAxSpearManStateWaitSee:// 様子見
		newAxSpearManState = new AxSpearManStateWaitSee();
		break;
	case kAxSpearManStateShakeOff:// 振り払い
		newAxSpearManState = new AxSpearManStateShakeOff();
		break;
	case kAxSpearManStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newAxSpearManState;
}
