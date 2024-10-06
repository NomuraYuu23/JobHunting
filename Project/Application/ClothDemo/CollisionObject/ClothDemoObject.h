#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/3D/Model/Model.h"

class ClothDemoObject
{

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="name">名前</param>
    virtual void Initialize(const std::string& name) = 0;

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

public: // アクセッサ

    /// <summary>
    /// デモに存在するか
    /// </summary>
    /// <returns></returns>
    bool GetExist() { return exist_; }

    /// <summary>
    /// デモに存在するか
    /// </summary>
    /// <param name="exist"></param>
    void SetExsit(bool exist) { exist_ = exist; }

    /// <summary>
    /// 名前
    /// </summary>
    /// <returns></returns>
    std::string GetName() { return name_; }

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

    // デモに存在するか
    bool exist_;

    // 名前
    std::string name_;

};

