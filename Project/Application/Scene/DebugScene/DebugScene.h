#pragma once

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../Skydome/Skydome.h"
#include "../../../Engine/Physics/Cloth/Cloth.h"

class DebugScene : public IScene
{

public: // メンバ関数

	~DebugScene();

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

private: // メンバ変数

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// スカイボックス
	uint32_t skyboxTextureHandle_ = 0;

	// 布
	std::unique_ptr<Cloth> cloth_;

};

