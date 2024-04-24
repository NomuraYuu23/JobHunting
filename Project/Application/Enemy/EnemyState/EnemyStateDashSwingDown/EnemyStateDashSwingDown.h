#pragma once
#include "../IEnemyState.h"
#include "../../../../Engine/Collider/Capsule/Capsule.h"
#include "../../EnemyAttack/EnemyAttack.h"

class EnemyStateDashSwingDown : public IEnemyState
{

public: // パブリック

	// コンボフェーズ
	enum ComboPhase {
		kPostureDrop, // 姿勢落とし
		kDash, // ダッシュ
		kCharge, // ため
		kSwing, // 振り下ろし
		kRecovery, // 残身
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
	/// 走る
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

	// あたり判定コライダー
	std::unique_ptr<Capsule> attackCollider_;

	// あたり判定ワールドトランスフォーム
	WorldTransform attackWorldTransform_;

	// 攻撃球の半径
	float attackRadius_;

	// 攻撃球と手の距離
	Vector3 attackLength_;

	// 攻撃球のプレイヤーからのローカル位置
	Vector3 attackCenter_;

	// 前フレームの攻撃球
	Vector3 prevAttackCenter_;

	// あたり判定を取るか
	bool isAttackJudgment_;

	// 媒介変数
	float parameter_;

	// フレーム数
	uint32_t periodFrame_;

	// フェーズ
	uint32_t inPhase_;

	// 向き制限
	Vector3 directionLimit = { 0.4f,0.4f,0.4f };

	// 攻撃
	std::unique_ptr<EnemyAttack> enemyAttack_;

public:

	Capsule* GetCollider() { return attackCollider_.get(); }

	bool GetIsAttackJudgment() { return isAttackJudgment_; }

};

