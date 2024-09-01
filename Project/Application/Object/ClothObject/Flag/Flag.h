#pragma once
#include "../BaseClothObject.h"

class FlagPole;

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

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="parent">親</param>
	void SetParent(FlagPole* parent) { parent_ = parent; }

private:

	// 親
	FlagPole* parent_;

};

