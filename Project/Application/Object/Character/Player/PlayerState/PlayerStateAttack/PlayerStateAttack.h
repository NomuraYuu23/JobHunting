#pragma once
#include <memory>
#include "../IPlayerState.h"
#include "../../../../../../Engine/Math/Vector/Vector3.h"
#include "../../../../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../../../../Engine/Collider/Capsule/Capsule.h"
#include "../../PlayerAttack/PlayerAttack.h"

class PlayerStateAttack : public IPlayerState
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
	static const int kComboNum = 2;
	// コンボ定数表
	static const std::array<ConstAttack, kComboNum> kConstAttaks;

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
	/// コンボ2段目 (左から右)
	/// </summary>
	void AttackCombo2nd();

	/// <summary>
	/// 攻撃判定出さない
	/// </summary>
	void DontAttack();

	/// <summary>
	/// 走る
	/// </summary>
	void Move();

private: // メンバ変数

	// 目指す向き
	Vector3 targetDirection_;

	// 目指す向きへの補間係数
	float targetAngleT_;

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

	// プレイヤーの攻撃情報
	PlayerAttack* attack_;

};

