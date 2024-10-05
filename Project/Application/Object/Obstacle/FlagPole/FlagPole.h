#pragma once
#include "../BaseObstacle.h"
#include "../../../../Engine/Physics/ClothGPU/ClothGPU.h"
class FlagPole :
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

	Vector3 GetUpperPart() { return upperPart_; }
	Vector3 GetLowerPart() { return lowerPart_; }

private: 

	/// <summary>
	/// 旗の位置を設定
	/// </summary>
	void FlagPosition();

private:

	// 旗の上部分
	Vector3 upperPart_{};
	
	// 旗の下部分
	Vector3 lowerPart_{};

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

