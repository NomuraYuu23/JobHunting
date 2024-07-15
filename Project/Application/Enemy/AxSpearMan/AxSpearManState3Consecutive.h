#pragma once
#include "IAxSpearManState.h"
#include "../BaseEnemyAttack.h"

class AxSpearManState3Consecutive :
    public IAxSpearManState
{

public: // パブリック

	// コンボフェーズ
	enum ComboPhase {
		kPosture1, // 構え1
		kkAttack1, // 攻撃1
		kRecovery1, // 残身
		kPosture2, // 構え2
		kkAttack2, // 攻撃2
		kRecovery2, // 残身
		kPosture3, // 構え3
		kkAttack3, // 攻撃3
		kRecovery3, // 残身
		kCountOfComboPhase,
	};

	// 攻撃用定数　時間<frame>
	struct ConstAttack {
		uint32_t time_[ComboPhase::kCountOfComboPhase];
		float speed_[ComboPhase::kCountOfComboPhase];
	};

private: // 静的メンバ変数

	// コンボ定数表
	static const ConstAttack kConstAttak;

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
	/// 攻撃初期化
	/// </summary>
	void AttackInitialize();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻撃判定出さない
	/// </summary>
	void DontAttack();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃終了
	/// </summary>
	void AttackPhaseFinished();

private: // メンバ変数

	// 目指す向き
	Vector3 targetDirection_;

	// 目指す向きへの補間係数
	float targetAngleT_;

	// 媒介変数
	float parameter_;

	// フレーム数
	uint32_t periodFrame_;

	// フェーズ
	uint32_t inPhase_;

	// 向き制限
	Vector3 directionLimit = { 0.4f,0.4f,0.4f };

	// 攻撃
	BaseEnemyAttack* attack_;

	// 攻撃のローカル位置
	Vector3 attackCenter_ = { 0.0f,0.0f,4.0f };

	// 攻撃の半径
	float attackRadius_ = 2.0f;

};

