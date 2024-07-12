#pragma once
#include "../../../Engine/Object/MeshObject.h"

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
    /// 親設定
    /// </summary>
    /// <param name="parentNodeData"></param>
    /// <param name="parentMatrix"></param>
    void SetParent(NodeData* parentNodeData, Matrix4x4* parentMatrix);

private:

    // 親のノード(持っている部分)
    NodeData* parentNodeData_;

    // 親のノード(持っている部分)
    NodeData* rootNodeData_;

    // 親の行列
    Matrix4x4* parentMatrix_;

};

