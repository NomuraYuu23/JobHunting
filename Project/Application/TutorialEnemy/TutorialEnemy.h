#pragma once
#include "../../Engine/Collider/Collider.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../../Engine/Collider/ColliderShape.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/Animation/Animation.h"

/// <summary>
/// エネミーのモーション一覧
/// </summary>
enum TutorialEnemyMotionIndex {
	kTutorialEnemyMotionStand, // 通常時
	kTutorialEnemyMotionDamage, // ダメージ時
	kTutorialEnemyMotionIndexOfCount // 数
};

class TutorialEnemy
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private: // ベースのメンバ変数

	// マテリアル
	std::unique_ptr<Material> material_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// コマンドを受け取るか
	bool receiveCommand_;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0x00000002;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xfffffffd;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;

private: // ステート関数

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInitialize();

	/// <summary>
	/// ステート更新
	/// </summary>
	void StateUpdate();

private: // ステート変数

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;

private: // パーツ構成関数

	/// <summary>
	/// パーツ初期化
	/// </summary>
	void PartInitialize();

	/// <summary>
	/// コライダー初期化
	/// </summary>
	void ColliderInitialize();

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimationUpdate();

private: // パーツ,アニメーション変数

	// モデル
	Model* model_;

	// コライダー (ダメージを受ける側、位置が被らない用)
	std::unique_ptr<Capsule> collider_;

	// コライダー用半径
	float colliderRadiuses_;

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	//ノードアニメーション
	Animation animation_;

	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;


private:  // パーツ,アニメーション定数

	// オブジェクト名
	const std::string objectName_ = "TutorialEnemy";

	// モーション名
	const std::array<const std::string, TutorialEnemyMotionIndex::kTutorialEnemyMotionIndexOfCount> motionNames_ = {
		"Stand",
		"Damage",
	};

private: // エネミーデータ

	// 高さ
	float height_ = 3.0f;

	// 幅
	float width_ = 2.0f;

	// プレイヤー
	Player* player_ = nullptr;

private: // 衝突処理

	void OnCollisionPlayerAttack(ColliderParentObject colliderPartner, const CollisionData& collisionData);

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

	void SetHeight(float height) { height_ = height; }

	float GetWidth() { return width_; }

	ColliderShape GetCollider() { return collider_.get(); }

	void SetPlayer(Player* player) { player_ = player; }

	Player* GetPlayer() { return player_; }

	uint32_t GetCurrentStateNo() { return currentStateNo_; }

	uint32_t GetSerialNumber() { return serialNumber_; }

};

