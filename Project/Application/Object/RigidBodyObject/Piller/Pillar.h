#pragma once
#include "../BaseRigidBodyObject.h"
#include "../../Obstacle/Pillar/PillarFoundation.h"
class Pillar :
    public BaseRigidBodyObject
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) override;

public:

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	/// <param name="colliderPartner">衝突相手</param>
	void Damage(uint32_t damage, ColliderParentObject colliderPartner);

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="parent">親</param>
	void SetParent(PillarFoundation* parent);

private:

	// 耐久値初期値
	static const uint32_t kInitDurable_ = 3;

	// 壊れたか
	bool broken_;

	// 耐久値
	int32_t durable_;

	// 親
	PillarFoundation* parent_;

	// コライダーの位置
	Vector3 colliderAddPos = { 0.0f,3.0f,0.0f };

};

