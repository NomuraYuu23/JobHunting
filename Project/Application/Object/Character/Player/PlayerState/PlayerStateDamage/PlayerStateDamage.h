#pragma once
#include "../IPlayerState.h"
class PlayerStateDamage :
    public IPlayerState
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

private: // メンバ変数

	// フレームカウント
	uint32_t frameCount_;

	// 終了フレームカウント
	const uint32_t frameEnd_ = 25;

};

