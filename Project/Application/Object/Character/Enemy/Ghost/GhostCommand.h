#pragma once
#include <cstdint>

class Ghost;

class GhostCommand
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ghost">ゴースト</param>
	void Initialize(Ghost* ghost);

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private: 

	// ゴースト
	Ghost* ghost_;

public:

	// ゴースト
	void SetGhost(Ghost* ghost) { ghost_ = ghost; }

};

