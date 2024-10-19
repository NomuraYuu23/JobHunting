#pragma once
#include "../IPlayerState.h"
#include "../../../../../../Engine/Math/Vector/Vector3.h"
#include "../../../../../../Engine/3D/Transform/WorldTransform.h"

class PlayerStateAvoidance : public IPlayerState
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

	// 回避速度
	float spped_;

	// 回避フレーム
	uint32_t frame_;

	// フレームカウント
	uint32_t frameCount_;

	// 移動開始フレーム
	uint32_t moveStartFrame_ = 0;

};

