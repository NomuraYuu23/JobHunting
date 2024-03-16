#pragma once
#include "../IPlayerState.h"
#include "../../../../Engine/Math/Vector3.h"
#include "../../../../Engine/3D/WorldTransform.h"

class PlayerStateRecovery : public IPlayerState
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

	/// <summary>
	/// 走る
	/// </summary>
	void Move(Vector3& move, WorldTransform* worldTransform);

	/// <summary>
	/// 回復
	/// </summary>
	void Recovery();

private: // メンバ変数

	// 移動速度
	float spped_;

	// 回復フレーム
	uint32_t frame_;

	// フレームカウント
	uint32_t frameCount_;

	// 目指す向き
	Vector3 targetDirection_;

	// 目指す向きへの補間係数
	float targetAngleT_;

	// 回復量
	uint32_t recoverySpeed_;

};

