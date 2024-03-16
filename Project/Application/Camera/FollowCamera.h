#pragma once
#include "../../../Engine/3D/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"

class FollowCamera : public BaseCamera{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	void SetDestinationAngle(const Vector3& destinationAngle) { destinationAngle_ = destinationAngle; }

private: // メンバ関数

	// 追従対象からのオフセットを計算する
	Vector3 OffsetCalc() const;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private:

	//追従対象
	const WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	Vector3 destinationAngle_ = { 0.0f,0.0f,0.0f };

	float moveRate_ = 0.2f;
	float rotateRate_ = 0.1f;

	float offsetLength_ = -100.0f;

};

