#pragma once
#include <string>
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Enemy/BaseEnemy.h"
#include "../../Engine/Object/BaseObjectManager.h"

class BossSystem
{

public:

	void Initialize(BaseObjectManager* objectManager);

	void Update();
	
	bool GetIsBossBattle() { return isBossBattle_; }

private:

	// ボスの名前
	const std::string kBossName_ = "AxSpearMan00";

	// ボス戦が始まる距離
	const float kBattleDistance_ = 10.0f;

	// ボス戦中か
	bool isBossBattle_;

	// ボス
	BaseEnemy* boss_;

	// 確認までのフレーム
	uint32_t checkFrame_ = 0;
	uint32_t checkFrameMax_ = 10;
	

};

