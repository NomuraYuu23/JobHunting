#pragma once
#include "IGhostState.h"
class GhostStateDead : public IGhostState
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

	// フレーム
	uint32_t frame_;

	// フレームカウント
	uint32_t frameCount_;


};

