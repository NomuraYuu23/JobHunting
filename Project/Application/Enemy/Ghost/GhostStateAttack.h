#pragma once
#include <array>
#include "IGhostState.h"
#include "../BaseEnemyAttack.h"

class GhostStateAttack : public IGhostState
{

public: // パブリック

	// コンボフェーズ
	enum ComboPhase {
		kAnticipation,
		kCharge,
		kSwing,
		kRecovery,
		kCountOfComboPhase,
	};

	// 攻撃用定数　時間<frame>
	struct ConstAttack {
		uint32_t time_[ComboPhase::kCountOfComboPhase];
		float speed_[ComboPhase::kCountOfComboPhase];
	};

private: // 静的メンバ変数

	// コンボの数
	static const int kComboNum = 1;
	// コンボ定数表
	static const std::array<ConstAttack, kComboNum> kConstAttacks;

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
	/// コンボフェーズ継続
	/// </summary>
	void AttackComboContinuationJudgment();

	/// <summary>
	/// コンボフェーズ終了
	/// </summary>
	void AttackComboPhaseFinished();

	/// <summary>
	/// コンボ終了判定
	/// </summary>
	void AttackComboFinished();

	/// <summary>
	/// コンボ1段目 (右から左)
	/// </summary>
	void AttackCombo1st();

	/// <summary>
	/// 攻撃判定出さない
	/// </summary>
	void DontAttack();

	/// <summary>
	/// 走る
	/// </summary>
	void Move();

private:

	// 媒介変数
	float parameter_;

	// フレーム数
	uint32_t periodFrame_;

	// コンボインデックス
	uint32_t comboIndex_;

	// コンボフェーズ
	uint32_t inComboPhase_;

	// コンボネクスト
	bool comboNext_;

	// 攻撃情報
	BaseEnemyAttack* attack_;

};

