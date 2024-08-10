#pragma once

#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"
#include "../../../Engine/3D/DrawLine.h"
#include "../../../Engine/Physics/RigidBody.h"
#include "../Enemy/BaseEnemyAttack.h"

class BaseEnemy : public MeshObject
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(BaseCamera& camera) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	virtual void ImGuiDraw() {};

	/// <summary>
	/// デバッグ描画マッピング
	/// </summary>
	void DebugDrawMap(DrawLine* drawLine);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	virtual void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) = 0;

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

protected: // ベースのメンバ変数

	// アタック
	std::unique_ptr<BaseEnemyAttack> attack_;

protected: // ステート関数

	/// <summary>
	/// ステート初期化
	/// </summary>
	virtual void StateInitialize() = 0;

	/// <summary>
	/// ステート更新
	/// </summary>
	virtual void StateUpdate() = 0;

	/// <summary>
	/// コマンド初期化
	/// </summary>
	virtual void CommandInitialize() = 0;

protected: // ステート変数

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;

	// コマンドを受け取るか
	bool receiveCommand_;

	// 割り込みコマンドがあるか
	bool interruptCommand_;

protected: // パーツ構成関数

	/// <summary>
	/// パーツ初期化
	/// </summary>
	virtual void PartInitialize() = 0;

	/// <summary>
	/// コライダー更新
	/// </summary>
	virtual void  ColliderUpdate() = 0;

	/// <summary>
	/// アニメーション更新
	/// </summary>
	virtual void  AnimationUpdate() = 0;

protected: // パーツ,アニメーション変数

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	//ノードアニメーション
	Animation animation_;

	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

protected: // 衝突処理

	/// <summary>
	/// 障害物との衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData);

protected:

	//hp
	int32_t hp_;
	uint32_t initHp_;

	// 剛体
	RigidBody rigidBody_;

	// 剛体使うか
	bool usedRigidBody_;

	// 反発係数
	float coefficientOfRestitution = 0.0f;

protected:

	/// <summary>
	/// Hp初期化
	/// </summary>
	void HPInit(uint32_t initHp);

public:

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage"></param>
	virtual void Damage(uint32_t damage);

	/// <summary>
	/// HPの割合
	/// </summary>
	/// <returns></returns>
	float RatioHP();

public: // アクセッサ

	BaseEnemyAttack* GetAttack() { return attack_.get(); }

	int32_t GetHp() { return hp_; }

	uint32_t GetInitHp() { return initHp_; }

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	uint32_t GetSerialNumber() { return serialNumber_; }

	Animation* GetAnimationAddress() { return &animation_; }

	LocalMatrixManager* GetLocalMatrixManager() { return localMatrixManager_.get(); }

	RigidBody* GetRigidBody() { return &rigidBody_; };

	void SetUsedRigidBody(bool usedRigidBody) { usedRigidBody_ = usedRigidBody; }

};

