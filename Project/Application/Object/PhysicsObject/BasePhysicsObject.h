#pragma once
#include "../../../Engine/Object/MeshObject.h"
class BasePhysicsObject :
    public MeshObject
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(LevelData::MeshData* data);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(BaseCamera& camera);

	/// <summary>
	/// ImGui描画
	/// </summary>
	virtual void ImGuiDraw();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	virtual void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

public:

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

};

