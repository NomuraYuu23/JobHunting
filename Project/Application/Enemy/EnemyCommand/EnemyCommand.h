#pragma once
#include <cstdint>

class EnemyCommand
{

public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static EnemyCommand* GetInstance();

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private:
	EnemyCommand() = default;
	~EnemyCommand() = default;
	EnemyCommand(const EnemyCommand&) = delete;
	const EnemyCommand& operator=(const EnemyCommand&) = delete;

};

