#pragma once
#include "../BaseRigidBodyObject.h"
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

private:

	// 耐久値初期値
	static const uint32_t kInitDurable_ = 3;

	// 壊れたか
	bool broken_;

	// 耐久値
	uint32_t durable_;

};

