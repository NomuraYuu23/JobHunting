#pragma once
#include "../../Engine/Object/MeshObject.h"
#include "../../Engine/Physics/RigidBody.h"

class BaseWeapon : public MeshObject
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
    /// 親の名前
    /// </summary>
    /// <returns></returns>
    std::string GetParentName() { return parentName_; }

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    virtual void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    /// <summary>
    /// 衝突マネージャー登録
    /// </summary>
    /// <param name="collisionManager"></param>
    virtual void CollisionListRegister(CollisionManager* collisionManager) override;

    /// <summary>
    /// 衝突マネージャー登録
    /// </summary>
    /// <param name="collisionManager"></param>
    /// <param name="colliderDebugDraw"></param>
    virtual void CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw) override;

protected:

    /// <summary>
    /// 親あるとき更新
    /// </summary>
    virtual void WithParentsUpdate() = 0;

    /// <summary>
    /// 親がない時更新
    /// </summary>
    virtual void ParentlessUpdate() = 0;

    /// <summary>
    /// 剛体初期化
    /// </summary>
    virtual void RigidBodyInitialize();

    /// <summary>
    /// 剛体更新
    /// </summary>
    virtual void RigidBodyUpdate();

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    virtual void ColliderInitialize() = 0;

    /// <summary>
    /// コライダーの更新
    /// </summary>
    virtual void ColliderUpdate() = 0;

private:

    /// <summary>
    /// 地面との当たり判定
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollisionGround(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    void ApplyForce(const Vector3& pointOfAction, const Vector3& force);

protected: // メンバ変数

    // 親のノード(持っている部分)
    NodeData* parentNodeData_;

    // 親の行列
    Matrix4x4* parentMatrix_;

    // 回転
    Vector3 rotate_ = { 0.0f, -1.57f, 0.0f };

    // 親の名前
    std::string parentName_;

    // 剛体
    RigidBody rigidBody_;

    // 反発係数
    float coefficientOfRestitution = 0.0f;

};

