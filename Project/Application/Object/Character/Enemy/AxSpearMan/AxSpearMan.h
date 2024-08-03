#pragma once
#include "../BaseEnemy.h"

#include "AxSpearManCommand.h" // コマンド
#include "IAxSpearManState.h" // ステート
#include "AxSpearManWeapon.h" // 武器
#include "AxSpearManBeamAttack.h" // ビーム(コライダー)
#include "AxSpearManBeam.h"

class Player;

/// <summary>
/// モーション一覧
/// </summary>
enum AxSpearManMotionIndex {
	kAxSpearManMotionIndexWait, // 通常時
	kAxSpearManMotionDashSwingDown, // ダッシュ振り下ろし
	kAxSpearManMotionWaitSee, // 様子見
	kAxSpearManMotionShakeOff, // 振り払い
	kAxSpearManMotion3Consecutive, // 3連撃
	kAxSpearManMotionBeam, // ビーム
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
	/// ビーム初期化
	/// </summary>
	void BeamInitialize();

	/// <summary>
	/// ビーム更新
	/// </summary>
	void BeamUpdate();

private:  // パーツ,アニメーション定数

	// オブジェクト名
	const std::string objectName_ = "AxSpearMan";

	// モーション名
	const std::array<const std::string, AxSpearManMotionIndex::kAxSpearManMotionIndexOfCount> motionNames_ = {
		"Wait",
		"DashSwingDown",
		"WaitSee",
		"ShakeOff",
		"3Consecutive",
		"Beam",
	};

private:

	// 高さ
	float height_;

	// 前フレームの座標
	Vector3 prePosition_;

	// プレイヤー
	Player* player_;

	// ビーム
	std::unique_ptr<AxSpearManBeam> beam_;

	// ビーム(コライダー)
	std::unique_ptr<AxSpearManBeamAttack> beamAttack_;

	// ビームを表示するか
	bool isBeamDraw_;

public:

	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer() { return player_; }

	AxSpearManBeamAttack* GetBeamAttack() { return beamAttack_.get(); }

	void SetIsBeamDraw(bool isBeamDraw) { isBeamDraw_ = isBeamDraw; }

};

