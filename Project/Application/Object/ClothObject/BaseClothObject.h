#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Physics/StructuralSpring.h"
class BaseClothObject :
    public MeshObject
{

public: // 定数

	// 余分な行列
	static const uint32_t kExtraMatrixNum = 2;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data">メッシュデータ</param>
	virtual void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void Draw(BaseCamera& camera) override;

	/// <summary>
	/// デバッグ描画用マッピング
	/// </summary>
	/// <param name="drawLine">線描画ポインタ</param>
	virtual void DebugDrawMap(DrawLine* drawLine);

	/// <summary>
	/// アンカー設定
	/// </summary>
	/// <param name="pointIndex">インデックス</param>
	/// <param name="fixPoint">固定するか</param>
	virtual void SetAnchor(uint32_t y, uint32_t x, bool fixPoint);

	/// <summary>
	/// 位置設定
	/// </summary>
	/// <param name="pointIndex">インデックス</param>
	/// <param name="position">位置</param>
	virtual void SetPosition(uint32_t y, uint32_t x, const Vector3& position);

	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	virtual void CollisionListRegister(CollisionManager* collisionManager);

	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	/// <param name="colliderDebugDraw"></param>
	virtual void CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw);

public: // アクセッサ

	std::vector<StructuralSpring> GetSpring() { return structuralSpring_; }

protected:

	//バネ
	std::vector<StructuralSpring> structuralSpring_;

	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

	// アンカーの初期ポジション
	Vector3 anchorInitPosition_ = {};

	// 自然長
	float naturalLengthY_ = 0.1f;
	float naturalLengthX_ = 0.1f;

	// 剛性。バネ定数k
	float stiffness_ = 750.0f;

	// 減衰係数
	float dampingCoefficient_ = 2.0f;

	// 質量(質点)
	float mass_ = 0.5f;

};

