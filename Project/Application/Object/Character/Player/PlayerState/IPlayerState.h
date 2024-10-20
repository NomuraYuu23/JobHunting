#pragma once
#include <cstdint>
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"

class Player;

//プレイヤーの状態名
enum PlayerState {
	kPlayerStateRoot, // 通常
	kPlayerStateAvoidance, // 回避
	kPlayerStateRecovery, // 回復
	kPlayerStateGuard, // ガード
	kPlayerStateAttack, // 攻撃
	kPlayerStateDamage, // ダメージ
	kPlayerStateOfCount // 数
};

/// <summary>
/// プレイヤーの状態基底クラス
/// </summary>
class IPlayerState
{
	
protected:

	// プレイヤーの状態番号
	static uint32_t playerStateNo_;

	// プレイヤーのモーション番号
	static uint32_t playerMotionNo_;

	// プレイヤー
	static Player* player_;

	//インスタンス
	static Input* input_;

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
	virtual ~IPlayerState() = default;

	// プレイヤーの状態番号
	uint32_t GetPlaryerStateNo() { return playerStateNo_; }

	// プレイヤーのモーション番号
	uint32_t GetPlaryerMotionNo() { return playerMotionNo_; }

	// プレイヤーセット
	void SetPlayer(Player* player) { player_ = player; }

};

