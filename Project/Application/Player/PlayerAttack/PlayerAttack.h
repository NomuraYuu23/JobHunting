#pragma once
#include "../../../Engine/Collider/Capsule/Capsule.h"
#include "../../../Engine/Collision/CollisionData.h"
#include "../../../Engine/ContactRecord/ContactRecord.h"
#include "../../../Engine/Collision/CollisionManager.h"
#include "../../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"

class PlayerAttack
{

public: // 変数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parent">親</param>
	void Initialize(WorldTransform* parent);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 停止
	/// </summary>
	void Stop();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

	/// <summary>
	/// 接触履歴抹消
	/// </summary>
	void ClearContactRecord();

	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionListRegister(CollisionManager* collisionManager);

	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw);
	
public: // アクセッサ

	/// <summary>
	/// 接触履歴取得
	/// </summary>
	/// <returns></returns>
	ContactRecord& GetContactRecord() { return contactRecord_; }

	/// <summary>
	/// あたり判定コライダー
	/// </summary>
	/// <returns></returns>
	ColliderShape* GetCollider() { return collider_.get(); }

	/// <summary>
	/// あたり判定を取るか
	/// </summary>
	/// <param name="isAttackJudgment"></param>
	void SetIsAttackJudgment(bool isAttackJudgment) { isAttackJudgment_ = isAttackJudgment; }

	/// <summary>
	/// あたり判定を取るか
	/// </summary>
	bool GetIsAttackJudgment() { return isAttackJudgment_; }

private:

	/// <summary>
	/// 衝突処理 敵
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private:

	// 接触履歴
	ContactRecord contactRecord_;

	// ダメージ
	uint32_t damage_ = 1;

	// あたり判定コライダー
	std::unique_ptr<ColliderShape> collider_;

	// あたり判定ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 攻撃球の半径
	float radius_;

	// 攻撃球のプレイヤーからのローカル位置
	Vector3 center_;

	// あたり判定を取るか
	bool isAttackJudgment_;

};

