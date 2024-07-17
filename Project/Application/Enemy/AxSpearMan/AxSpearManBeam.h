#pragma once
#include "../../../Engine/Object/MeshObject.h"

class AxSpearManBeam :
    public MeshObject
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
    void Update(
        const Vector3& position,
        const Matrix4x4& rotateMatrix,
        const Vector3& size);

private:

    // uv用トランスフォーム
    EulerTransform uvTransform{};

    // uvの変更速度
    const float kUvTransformSpeed = 0.05f;

    // 位置
    Vector3 position_;

    // 回転行列
    Matrix4x4 rotateMatrix_;

    //大きさ
    Vector3 size_;

};

