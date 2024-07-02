#pragma once
#include <cstdint>

class Ghost;

//プレイヤーの状態名
enum GhostState {
	kGhostStateRoot, // 通常
	kGhostStateChase, // 追跡
	kGhostStateAttack, // 攻撃
	kGhostStateOfCount // 数
};

class IGhostState
{

protected:

	// プレイヤーの状態番号
	static uint32_t ghostStateNo_;

	// プレイヤーのモーション番号
	static uint32_t ghostMotionNo_;

	// プレイヤー
	static Ghost* ghost_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IGhostState() = default;

	// 状態番号
	uint32_t GetGhostStateNo() { return ghostStateNo_; }

	// モーション番号
	uint32_t GetGhostMotionNo() { return ghostMotionNo_; }

	// 自身をセット
	void SetGhost(Ghost* ghost) { ghost_ = ghost; }

};

