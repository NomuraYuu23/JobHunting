#pragma once
#include "../../../Engine/Collider/ColliderShape.h"
#include "../../../Engine/Collision/CollisionData.h"
#include "../../../Engine/ContactRecord/ContactRecord.h"
#include "../../../Engine/Collision/CollisionManager.h"
#include "../../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"
#include "../../../GPUParticle/BloadParticle.h"

class BaseEnemyAttack
{

public: // 変数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parent">親</param>
	virtual void Initialize(WorldTransform* parent);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

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

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="commandList">コマンドリスト</param>
	void ParticleDraw(ID3D12GraphicsCommandList* commandList, BaseCamera& camera);

	/// <summary>
	/// パーティクル更新
	/// </summary>
	void ParticleUpdate();

	/// <summary>
	/// 親のワールドポジション
	/// </summary>
	/// <returns></returns>
	Vector3 GetParentPos();

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

	/// <summary>
	/// 攻撃球の半径
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float radius) { radius_ = radius; }

	/// <summary>
	/// 攻撃球のプレイヤーからのローカル位置
	/// </summary>
	/// <param name="center"></param>
	void SetCenter(const Vector3& center) { center_ = center; }

	/// <summary>
	/// ワールドトランスフォームアドレス取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

private:

	/// <summary>
	/// 衝突処理 敵
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionPlayer(ColliderParentObject colliderPartner, const CollisionData& collisionData);

protected:

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

	// パーティクル
	std::unique_ptr<BloadParticle> bloadParticle_;

	//パーティクルタイム
	float particleTime_ = 0.0f;
	const float particleTimeMax_ = 0.3f;

};

