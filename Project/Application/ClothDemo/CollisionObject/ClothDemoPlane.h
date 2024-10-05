#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/3D/Model/Model.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPUCollision.h"

class ClothDemoPlane
{

public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void Draw(BaseCamera& camera);

    /// <summary>
    /// ImGui描画
    /// </summary>
    void ImGuiDraw();

    ClothGPUCollision::Plane GetData() { return data_; }

private:

	// トランスフォーム
	WorldTransform worldTransform_;

    // ファイル名前
    std::string fileName_;

    // ディレクトリパス
    std::string directoryPath_;

    // モデル
    std::unique_ptr<Model> model_ = nullptr;

    // マテリアル
    std::unique_ptr<Material> material_;

    // ClothGPUに渡すデータ
    ClothGPUCollision::Plane data_;

    // 画面ちらつかないようの値
    float screenDoesNotFlickerValue_;

    // 大きさ
    const Vector3 kScale_ = { 10.0f,10.0f,10.0f };

};
