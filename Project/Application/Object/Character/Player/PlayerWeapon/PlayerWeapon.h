#pragma once
#include "../../../Weapon/BaseWeapon.h"

class Player;

class PlayerWeapon :
    public BaseWeapon
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
    void SetParent(Player* parent);

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) override;

private:

    /// <summary>
    /// 親あるとき更新
    /// </summary>
    void WithParentsUpdate() override;

    /// <summary>
    /// 親がない時更新
    /// </summary>
    void ParentlessUpdate() override;

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    void ColliderInitialize() override;

    /// <summary>
    /// コライダーの更新
    /// </summary>
    void ColliderUpdate() override;

private:

    // 親
    Player* parent_;

};

