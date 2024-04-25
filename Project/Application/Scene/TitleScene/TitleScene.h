#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../AudioManager/TitleAudioManager.h"

#include "../../Skydome/Skydome.h"

class TitleScene : public IScene
{

public: // メンバ関数

	~TitleScene();

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

private:

	/// <summary>
	/// 音量
	/// </summary>
	void LowerVolumeBGM();

private: // メンバ変数

	
	// オーディオマネージャー
	std::unique_ptr<TitleAudioManager> audioManager_;

	bool isDecreasingVolume = true;

	// タイトル
	std::unique_ptr<Sprite> titleSprite_;
	uint32_t titleTextureHandle_;

	// 点滅用媒介変数
	float titleAlphaT_;
	// 点滅用媒介変数速度
	float titleAlphaTSpeed_;
	// 点滅用媒介変数は増えるか
	bool titleItIncreaseAlphaT_;
	// 色
	Vector4 titleColor_;

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

