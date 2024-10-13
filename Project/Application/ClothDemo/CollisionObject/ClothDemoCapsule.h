#pragma once
#include "ClothDemoObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPUCollision.h"

class ClothDemoCapsule :
    public ClothDemoObject
{

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="name">名前</param>
    void Initialize(const std::string& name) override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void Draw(BaseCamera& camera) override;

    /// <summary>
    /// ImGui描画
    /// </summary>
    void ImGuiDraw() override;

    /// <summary>
    /// データ取得
    /// </summary>
    /// <returns></returns>
    ClothGPUCollision::Capsule GetData() { return data_; }

private:

    // ClothGPUに渡すデータ
    ClothGPUCollision::Capsule data_;

    // 画面ちらつかないようの値
    float screenDoesNotFlickerValue_;

    // トランスフォーム、カプセルの下用
    WorldTransform diffWorldTransform_;

};

