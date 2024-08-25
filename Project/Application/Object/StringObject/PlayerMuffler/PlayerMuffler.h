#pragma once
#include "../BaseStringObject.h"
class PlayerMuffler :
    public BaseStringObject
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
	/// <param name="player"></param>
	void SetParent(Player* player);

private:

	// プレイヤー
	Player* player_ = nullptr;

	// プレイヤーからのローカル位置
	Vector3 localPosition_ = { 0.0f,0.0f,0.0f };

};

