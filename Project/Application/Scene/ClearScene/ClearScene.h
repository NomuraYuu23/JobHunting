#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../Skydome/Skydome.h"

class ClearScene : public IScene
{

public: // メンバ関数

	~ClearScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private: // メンバ変数

	// クリア
	std::unique_ptr<Sprite> clearSprite_;
	uint32_t clearTextureHandle_;

	// ボタン
	std::unique_ptr<Sprite> buttonSprite_;
	uint32_t buttonTextureHandle_;
	// 点滅用媒介変数
	float buttonAlphaT_;
	// 点滅用媒介変数速度
	float buttonAlphaTSpeed_;
	// 点滅用媒介変数は増えるか
	bool buttonItIncreaseAlphaT_;
	// 色
	Vector4 buttonColor_;


};

