#pragma once
#include "../BaseEnemy.h"

#include "AxSpearManCommand.h" // コマンド
#include "IAxSpearManState.h" // ステート
#include "AxSpearManWeapon.h"

class Player;

/// <summary>
/// モーション一覧
/// </summary>
enum AxSpearManMotionIndex {
	kAxSpearManMotionIndexWait, // 通常時
	kEnemyMotionDashSwingDown, // ダッシュ振り下ろし
	kAxSpearManMotionIndexOfCount // 数
};


class AxSpearMan : public BaseEnemy
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(BaseCamera& camera) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) override;

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
	std::unique_ptr<AxSpearManCommand> command_;

	// ステート
	std::unique_ptr<IAxSpearManState> state_;

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

	/// <summary>
	/// 武器初期化
	/// </summary>
	void WeaponInitialize();

	/// <summary>
	/// 武器更新
	/// </summary>
	void WeaponUpdate();

private:  // パーツ,アニメーション定数

	// オブジェクト名
	const std::string objectName_ = "Ghost";

	// モーション名
	const std::array<const std::string, AxSpearManMotionIndex::kAxSpearManMotionIndexOfCount> motionNames_ = {
		"Wait",
	};

private:

	// 高さ
	float height_;

	// 前フレームの座標
	Vector3 prePosition_;

	// プレイヤー
	Player* player_;

	// 武器
	std::unique_ptr<AxSpearManWeapon> weapon_;

public:

	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer() { return player_; }

};

