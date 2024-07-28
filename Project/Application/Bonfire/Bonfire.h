#pragma once
#include "../../Engine/Object/MeshObject.h"
#include "../GPUParticle/BonfireParticle.h"
class Bonfire :
    public MeshObject
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
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera"></param>
	void ParticleDraw(BaseCamera& camera) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

public:

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

private:

	std::unique_ptr<BonfireParticle> bonfireParticle_ = nullptr;

};

