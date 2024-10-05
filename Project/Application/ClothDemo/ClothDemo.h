#pragma once

#include "../../../Engine/Physics/Cloth/Cloth.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"
#include "../../Engine/base/DxCommon/DirectXCommon.h"
#include "../../Engine/Input/Input.h"

class ClothDemo
{

public: // サブクラス

	/// <summary>
	/// 布固定
	/// </summary>
	enum FixedIndex {
		kFixedIndexEnd, // 端
		kFixedIndexTop, // 上
		kFixedIndexOfCount // 数
	};

public: // メンバ関数

	/// <summary>
	///  初期化
	/// </summary>
	/// <param name="directionalLight"></param>
	/// <param name="pointLightManager"></param>
	/// <param name="spotLightManager"></param>
	void Initilalize(
		DirectionalLight* directionalLight,
		PointLightManager* pointLightManager,
		SpotLightManager* spotLightManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera* camera);
	
	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private: // メンバ関数

	/// <summary>
	/// 布リセット
	/// </summary>
	/// <param name="fixedIndex">固定方法</param>
	void ClothReset(FixedIndex fixedIndex);

	/// <summary>
	/// 固定をはずす
	/// </summary>
	void RemoveFixation();

private: // 布固定関数

	/// <summary>
	/// 端固定
	/// </summary>
	void ClothFixedEnd();

	/// <summary>
	/// 上固定
	/// </summary>
	void ClothFixedTop();

private: // メンバ変数

	// 布
	std::unique_ptr<ClothGPU> clothGPU_;
	
	// 大きさ
	Vector2 clothScale_;
	
	// 分割数
	Vector2 clothDiv_;

	// リセット位置
	Vector3 resetPosition_;

	// 布固定関数群
	std::array<std::function<void()>, FixedIndex::kFixedIndexOfCount> clothFixedFunctions_;

	// デバイス
	DirectXCommon* dxCommon_;

	// 入力
	Input* input_;

};

