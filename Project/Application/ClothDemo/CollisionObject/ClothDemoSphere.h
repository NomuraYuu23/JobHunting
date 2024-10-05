#pragma once
#include "ClothDemoObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPUCollision.h"

class ClothDemoSphere :
    public ClothDemoObject
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
    ClothGPUCollision::Sphere GetData() { return data_; }

private:

    // ClothGPUに渡すデータ
    ClothGPUCollision::Sphere data_;

    // 画面ちらつかないようの値
    float screenDoesNotFlickerValue_;

};

