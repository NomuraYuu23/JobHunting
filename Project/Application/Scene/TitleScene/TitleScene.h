#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../Skydome/Skydome.h"
#include "../../../Engine/PostEffect/HSV/HSVFilter.h"

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

};
