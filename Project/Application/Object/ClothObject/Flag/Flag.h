#pragma once
#include "../BaseClothObject.h"

class Flag :
    public BaseClothObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data">メッシュデータ</param>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	// プレイヤーからのローカル位置
	Vector3 localPosition_ = { 0.0f,0.0f,0.0f };

};

