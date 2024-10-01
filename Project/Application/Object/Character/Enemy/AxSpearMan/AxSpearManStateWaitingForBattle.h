#pragma once
#include "IAxSpearManState.h"
class AxSpearManStateWaitingForBattle :
    public IAxSpearManState
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	// ボス戦が始まる距離
	const float kBattleDistance_ = 10.0f;


	// 確認までのフレーム
	uint32_t checkFrame_ = 0;
	uint32_t checkFrameMax_ = 10;
};

