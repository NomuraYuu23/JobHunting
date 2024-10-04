#pragma once
#include "../../../Engine/Collider/Collider.h"
#include "PlayerState/IPlayerState.h"
#include "PlayerState/PlayerStateFactory.h"
#include "PlayerCommand/PlayerCommand.h"
#include "PlayerAttack/PlayerAttack.h"
#include "../../../Engine/Collider/ColliderShape.h"
#include "../../../Engine/Collision/CollisionData.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../../../Engine/3D/Model/Model.h"
#include "../../../Engine/Animation/Animation.h"
#include "../../../Engine/3D/Line/DrawLine.h"
#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/MeshObject.h"
#include "../../../GPUParticle/FieldSparksParticle.h"
#include "../../../../Engine/Physics/ClothGPU/ClothGPU.h"

/// <summary>
/// プレイヤーのモーション一覧
/// </summary>
enum PlayerMotionIndex {
	kPlayerMotionWait, // 通常時
	kPlayerMotionRun, // 走行時
	kPlayerMotionAttack1st, // 攻撃時(1段目)
	kPlayerMotionAttack2nd, // 攻撃時(2段目)
	kPlayerMotionAvoidance, // 回避時
	kPlayerMotionRest, // 座る


	kPlayerMotionDash, // ダッシュ時
	kPlayerMotionWalk, // 歩行時
	kPlayerMotionGuard, // ガード時
	kPlayerMotionGuardWalk, // ガード歩行時
	kPlayerMotionRecovery, // 回復時
	kPlayerMotionIndexOfCount // 数
};

/// <summary>
/// プレイヤー
/// </summary>
class Player : public MeshObject
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

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
	/// デバッグ描画マッピング
	/// </summary>
	void DebugDrawMap(DrawLine* drawLine);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

	/// <summary>
	/// 衝突マネージャー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionListRegister(CollisionManager* collisionManager) override;

	/// <summary>
	/// 衝突マネージャー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	/// <param name="colliderDebugDraw"></param>
	void CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw) override;

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void ParticleDraw(BaseCamera& camera) override;

private: // ベースのメンバ変数

	// コマンド
	PlayerCommand* playerCommand_;

	// コマンドを受け取るか
	bool receiveCommand_;

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

	// ステート
	std::unique_ptr<IPlayerState> playerState_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;
	
	// ステートファクトリー
	PlayerStateFactory* playerStateFactory_;

private: // パーツ構成関数

	/// <summary>
	/// パーツ初期化
	/// </summary>
	void PartInitialize();

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimationUpdate();

private: // パーツ,アニメーション変数

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
	const std::string objectName_ = "Player";

	// モーション名
	const std::array<const std::string, PlayerMotionIndex::kPlayerMotionIndexOfCount> motionNames_ = {
		"Wait",
		"Run",
		"Attack1st",
		"Attack2nd",
		"Avoidance",
		"Rest",

		"Dash",
		"Walk",
		"Guard",
		"GuardWalk",
		"Recovery",
	};

private: // プレイヤーデータ

	// 高さ
	float height_ = 2.0f;

	// 幅
	float width_ = 1.0f;

	// カメラ
	BaseCamera* camera_ = nullptr;

	//hp
	int32_t hp_;

	// 初期HP
	uint32_t initHp_;

	// プレイヤーの攻撃情報
	std::unique_ptr<PlayerAttack> attack_;

	// 前フレームの位置
	Vector3 prePosition_;

	// フィールドパーティクル
	std::unique_ptr<FieldSparksParticle> fieldSparksParticle_;
	EmitterCS fieldSparksEmitter_;

	DirectXCommon* dxCommon_;

private:

	// マント
	std::unique_ptr<ClothGPU> cloak_;

	// 分割数
	Vector2 cloakDiv_;

	// 大きさ
	Vector2 cloakScale_;

	// 位置を設定する
	bool cloakIsPosSet_;

	NodeData* parentRightNodeData_;
	NodeData* parentLeftNodeData_;

	Vector3 cloakRightPos_;
	Vector3 cloakLeftPos_;

	Vector3 cloakRightLocalPos_;
	Vector3 cloakLeftLocalPos_;

private:

	/// <summary>
	/// 位置を設定
	/// </summary>
	void SetCloakPosition();

	/// <summary>
	/// ノード追従
	/// </summary>
	void CloakNodeFollowing();

	/// <summary>
	/// マント
	/// </summary>
	void CloakInitialize();

	/// <summary>
	/// マント更新
	/// </summary>
	void CloakUpdate();

public:

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage"></param>
	void Damage(uint32_t damage);

	/// <summary>
	/// HPの割合
	/// </summary>
	/// <returns></returns>
	float RatioHP();

private: // 衝突処理

	/// <summary>
	/// 敵との衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData);

	/// <summary>
	/// 障害物との衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData);

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetCamera(BaseCamera* camera) { camera_ = camera; }

	BaseCamera* GetCamera() { return camera_; }

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

	void SetHeight(float height) { height_ = height; }

	IPlayerState* GetPlayerState() { return playerState_.get(); }

	uint32_t GetCurrentStateNo() { return currentStateNo_; }

	int32_t GetHp() { return hp_; }

	uint32_t GetInitHp(){ return initHp_; }

	PlayerAttack* GetPlayerAttack() { return attack_.get(); }

	LocalMatrixManager* GetLocalMatrixManager() { return localMatrixManager_.get(); }

	void SetHP(uint32_t hp) { hp_ = hp; }

};
