#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/3D/Model/Model.h"

class ClothDemoObject
{

public:

    /// <summary>
    /// 初期化
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    virtual void Draw(BaseCamera& camera);

    /// <summary>
    /// ImGui描画
    /// </summary>
    virtual void ImGuiDraw() = 0;

protected:

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

};

