#pragma once
#include "../../../Engine/Particle/IParticle.h"
class BloadParticle : public IParticle
{

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BloadParticle() override {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	void Initialize(const Vector3& position, const Vector3& size) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="billBoardMatrix"></param>
	void Update(const Matrix4x4& billBoardMatrix) override;

private:

	// 速度
	Vector3 velocity_;

	// 加速度
	Vector3 acceleration_;

	// 色
	Vector4 redColor_ = { 236.0f / 255.0f, 63.0f / 255.0f, 49.0f / 255.0f, 1.0f };

};

