#pragma once
#include "../../../Engine/Collider/Capsule/Capsule.h"
#include "../../../Engine/Collision/CollisionData.h"
#include "../../../Engine/ContactRecord/ContactRecord.h"

class PlayerAttack
{

public: 

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
	/// 接触履歴取得
	/// </summary>
	/// <returns></returns>
	ContactRecord& GetContactRecord() { return contactRecord_; }

private:

	/// <summary>
	/// 衝突処理 敵
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData);

	/// <summary>
	/// 衝突処理 敵
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionTutorialEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private:

	// 接触履歴
	ContactRecord contactRecord_;

	// ダメージ
	uint32_t damage_ = 1;

};

