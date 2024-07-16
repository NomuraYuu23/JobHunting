#pragma once
#include "../BaseEnemyAttack.h"
class AxSpearManBeamAttack :
    public BaseEnemyAttack
{

public: // 変数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parent">親</param>
	void Initialize(WorldTransform* parent) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 回転設定
	/// </summary>
	/// <param name="rotate"></param>
	void SetRotate(const Vector3& rotate) { rotate_ = rotate; }

	/// <summary>
	/// 大きさ設定
	/// </summary>
	/// <param name="size">大きさ</param>
	void SetSize(const Vector3& size) { size_ = size; }

private:

	// 回転
	Vector3 rotate_;

	// 大きさ
	Vector3 size_;

};

