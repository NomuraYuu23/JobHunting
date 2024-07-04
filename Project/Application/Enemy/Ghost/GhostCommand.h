#pragma once
#include <cstdint>

class Player;
class Ghost;

class GhostCommand
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ghost">ゴースト</param>
	/// <param name="player">プレイヤー</param>
	void Initialize(Ghost* ghost, Player* player);

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private: 

	// ゴースト
	Ghost* ghost_;

	// プレイヤー
	Player* player_;

public:

	// ゴースト
	void SetGhost(Ghost* ghost) { ghost_ = ghost; }

	// プレイヤー
	void SetPlayer(Player* player) { player_ = player; }

};

