#pragma once
#include <memory>
#include <list>
#include "../../Engine/2D/Sprite.h"
#include "../Object/Character/Enemy/BaseEnemy.h"

/// <summary>
/// ロックオン
/// </summary>
class LockOn
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const std::list<BaseEnemy*>& enemies, Player* player, BaseCamera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// ロックオン対象の座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() const;

	/// <summary>
	/// ロックオン中か
	/// </summary>
	/// <returns></returns>
	bool ExistTarget() const { return target_ ? true : false; }

	/// <summary>
	/// リスタート
	/// </summary>
	void Restart();

private: // メンバ関数

	/// <summary>
	/// 範囲外判定
	/// </summary>
	bool OutOfRangeJudgment(Player* player, BaseCamera* camera);

private: // メンバ変数

	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

	// ロックオン対策
	BaseEnemy* target_ = nullptr;

	// 最小距離
	float minDistance_ = 0.0f;

	// 最大距離
	float maxDistance_ = 150.0f;

	// 角度範囲
	float angleRange_ = 0.7f;

};