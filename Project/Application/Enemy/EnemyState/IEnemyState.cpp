#include "IEnemyState.h"
#include "../Enemy.h"

uint32_t IEnemyState::enemyStateNo_ = 0;

uint32_t IEnemyState::enemyMotionNo_ = 0;

Enemy* IEnemyState::enemy_ = nullptr;

Input* IEnemyState::input_ = Input::GetInstance();
