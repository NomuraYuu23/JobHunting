#pragma once
#include "../BaseObstacle.h"
#include "../../../../Engine/Physics/ClothGPU/ClothGPU.h"
class FlagPole2 :
    public BaseObstacle
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
	void Draw(BaseCamera& camera) override;

	Vector3 GetRightPart() { return rightPart_; }
	Vector3 GetLeftPart() { return leftPart_; }

private:

	/// <summary>
	/// 旗の位置を設定
	/// </summary>
	void FlagPosition();

private:

	// 旗の右部分
	Vector3 rightPart_{};

	// 旗の左部分
	Vector3 leftPart_{};

	// 布
	std::unique_ptr<ClothGPU> cloth_;

	DirectXCommon* dxCommon_;

	// 分割数
	Vector2 div_;

	// 大きさ
	Vector2 scale_;

	// 位置を設定する
	bool isPosSet_;

	// コライダー追加分
	Vector3 colliderAdd_;

};

