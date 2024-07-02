#pragma once
#include "../BaseEnemy.h"
#include "GhostCommand.h"
#include "IGhostState.h"

class Player;

/// <summary>
/// モーション一覧
/// </summary>
enum GhostMotionIndex {
	kGhostMotionWait, // 通常時
	kGhostMotionMove, // 移動時
	kGhostMotionAttack, // 攻撃時
	kGhostMotionIndexOfCount // 数
};


class Ghost : public BaseEnemy
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	virtual void ImGuiDraw();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	virtual void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

protected: // ステート関数

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInitialize() override;

	/// <summary>
	/// ステート更新
	/// </summary>
	void StateUpdate() override;

	/// <summary>
	/// コマンド初期化
	/// </summary>
	void CommandInitialize() override;

private: //ステート

	// コマンド
	std::unique_ptr<GhostCommand> command_;

	// ステート
	std::unique_ptr<IGhostState> state_;

protected: // パーツ構成関数

	/// <summary>
	/// パーツ初期化
	/// </summary>
	void PartInitialize() override;

	/// <summary>
	/// コライダー更新
	/// </summary>
	void  ColliderUpdate() override;

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void  AnimationUpdate() override;

private:  // パーツ,アニメーション定数

	// オブジェクト名
	const std::string objectName_ = "Ghost";

	// モーション名
	const std::array<const std::string, GhostMotionIndex::kGhostMotionIndexOfCount> motionNames_ = {
		"Wait",
		"Move",
		"Attack"
	};

private: 

	// 高さ
	float height_;

	// 前フレームの座標
	Vector3 prePosition_;

	// プレイヤー
	Player* player_;

public:

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer() { return player_; }

};

