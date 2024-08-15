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

	// プレイヤーの名前
	const std::string kPlayerName_ = "000_Player";

	// ボスの名前
	const std::string kBossName_ = "AxSpearMan00";

	// ボス戦が始まる距離
	const float kBattleDistance_ = 10.0f;

	// ボス戦中か
	bool isBossBattle_;

	// プレイヤー
	Player* player_;

	// ボス
	BaseEnemy* boss_;

};

