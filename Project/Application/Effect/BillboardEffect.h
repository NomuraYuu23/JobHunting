#pragma once
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/3D/Model/Model.h"

class BillboardEffect
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="lifeTime">生存時間</param>
	void Initialize(Model* model, uint32_t textureHandle, float lifeTime);
	
	void Update(BaseCamera& camera);

	void Draw(BaseCamera& camera);

	void Reset();

public: // アクセッサ

	/// <summary>
	/// 初期位置
	/// </summary>
	/// <param name="initPosition">初期位置</param>
	void SetInitPosition(const Vector3& initPosition) { initPosition_ = initPosition; }

	/// <summary>
	/// 終了位置
	/// </summary>
	/// <param name="endPosition">終了位置</param>
	void SetEndPosition(const Vector3& endPosition) { endPosition_ = endPosition; }

	/// <summary>
	/// 初期大きさ
	/// </summary>
	/// <param name="initScale">初期大きさ</param>
	void SetInitScale(const Vector3& initScale) { initScale_ = initScale; }

	/// <summary>
	/// 終了大きさ
	/// </summary>
	/// <param name="endScale">終了大きさ</param>
	void SetEndScale(const Vector3& endScale) { endScale_ = endScale; }

	/// <summary>
	/// 初期透明度
	/// </summary>
	/// <param name="initAlpha">初期透明度</param>
	void SetInitAlpha(float initAlpha) { initAlpha_ = initAlpha; }

	/// <summary>
	/// 終了透明度
	/// </summary>
	/// <param name="endAlpha">終了透明度</param>
	void SetEndAlppha(float endAlpha) { endAlppha_ = endAlpha; }

	/// <summary>
	/// 色
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) { color_ = color; }

	/// <summary>
	/// 生存時間
	/// </summary>
	/// <param name="lifeTime"></param>
	void SetLifeTime(float lifeTime) { lifeTime_ = lifeTime; }

private: // 変数

	// モデル
	Model* model_;

	// マテリアル
	std::unique_ptr<Material> material_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// テクスチャハンドル
	uint32_t textureHnadle_;

	// 位置
	Vector3 position_;

	// 初期位置
	Vector3 initPosition_;

	// 終了位置
	Vector3 endPosition_;

	// 大きさ
	Vector3 scale_;

	// 初期大きさ
	Vector3 initScale_;

	// 終了大きさ
	Vector3 endScale_;

	// 透明度
	float alpha_;

	// 初期透明度
	float initAlpha_;

	// 終了透明度
	float endAlppha_;

	// 色
	Vector4 color_;

	// 補間係数
	float t_;

	// 生存時間
	float lifeTime_;

	// 死んでるか
	bool isDead_;

};

