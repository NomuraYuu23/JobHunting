#pragma once
#include "../../Engine/Collider/Collider.h"
#include "PlayerState/IPlayerState.h"
#include "PlayerState/PlayerStateFactory.h"
#include "PlayerCommand/PlayerCommand.h"
#include "../../Engine/Collider/ColliderShape.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/Animation/Animation.h"

/// <summary>
/// プレイヤーのモーション一覧
/// </summary>
enum PlayerMotionIndex {
	kPlayerMotionWait, // 通常時
	kPlayerMotionRun, // 走行時
	kPlayerMotionAttack1st, // 攻撃時(1段目)
	kPlayerMotionAttack2nd, // 攻撃時(2段目)
	kPlayerMotionAvoidance, // 回避時
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
class Player
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Model* weaponModel);

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

	// コマンド
	PlayerCommand* playerCommand_;

	// コマンドを受け取るか
	bool receiveCommand_;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0x00000001;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xfffffffe;

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
	const std::string objectName_ = "Player";

	// モーション名
	const std::array<const std::string, PlayerMotionIndex::kPlayerMotionIndexOfCount> motionNames_ = {
		"Wait",
		"Run",
		"Attack1st",
		"Attack2nd",
		"Avoidance",
		"Dash",
		"Walk",
		"Guard",
		"GuardWalk",
		"Recovery",
	};

private: // プレイヤーデータ

	// 高さ
	float height_ = 20.0f;

	// 幅
	float width_ = 10.0f;

	// カメラ
	BaseCamera* camera_ = nullptr;

	//hp
	int32_t hp_;

	// 初期HP
	uint32_t initHp_;

	// 死んだか
	bool isDead_;

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
	/// 
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData);

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetCamera(BaseCamera* camera) { camera_ = camera; }

	BaseCamera* GetCamera() { return camera_; }

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

	void SetHeight(float height) { height_ = height; }

	ColliderShape GetCollider() { return collider_.get(); }

	IPlayerState* GetPlayerState() { return playerState_.get(); }

	uint32_t GetCurrentStateNo() { return currentStateNo_; }

	uint32_t GetSerialNumber() { return serialNumber_; }

	bool GetIsDead() { return isDead_; }

	int32_t GetHp() { return hp_; }

	uint32_t GetInitHp(){ return initHp_; }

};
