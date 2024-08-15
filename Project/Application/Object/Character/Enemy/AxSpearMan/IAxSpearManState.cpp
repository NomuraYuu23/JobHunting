#include "IAxSpearManState.h"

uint32_t IAxSpearManState::axSpearManStateNo_ = kAxSpearManStateWaitingForBattle;

uint32_t IAxSpearManState::axSpearManMotionNo_ = 0;

AxSpearMan* IAxSpearManState::axSpearMan_ = nullptr;
