#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/3D/Model/Model.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPUCollision.h"
#include "ClothDemoObject.h"

class ClothDemoPlane : public ClothDemoObject
{

public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// ImGui描画
    /// </summary>
    void ImGuiDraw() override;

    /// <summary>
    /// データ取得
    /// </summary>
    /// <returns></returns>
    ClothGPUCollision::Plane GetData() { return data_; }

private:

    // ClothGPUに渡すデータ
    ClothGPUCollision::Plane data_;

    // 画面ちらつかないようの値
    float screenDoesNotFlickerValue_;

    // 大きさ
    const Vector3 kScale_ = { 10.0f,10.0f,10.0f };

};
