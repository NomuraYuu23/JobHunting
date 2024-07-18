#pragma once
#include "../../../Engine/Object/MeshObject.h"

class AxSpearMan;

class AxSpearManWeapon : public MeshObject
{

public: 

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 親
    /// </summary>
    /// <param name="parent"></param>
    void SetParent(AxSpearMan* parent);

    /// <summary>
    /// 親の名前
    /// </summary>
    /// <returns></returns>
    std::string GetParentName() { return parentName_; }

private:
    
    /// <summary>
    /// 親あるとき更新
    /// </summary>
    void WithParentsUpdate();

    /// <summary>
    /// 親がない時更新
    /// </summary>
    void ParentlessUpdate();

private:

    // 親
    AxSpearMan* parent_;

    // 親のノード(持っている部分)
    NodeData* parentNodeData_;

    // 親の行列
    Matrix4x4* parentMatrix_;

    // 回転
    Vector3 rotate_ = {0.0f, -1.57f, 0.0f};

    // 親の名前
    std::string parentName_;

};

