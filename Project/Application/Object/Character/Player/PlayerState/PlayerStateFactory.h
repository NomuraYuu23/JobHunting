#pragma once
#include "IPlayerState.h"

/// <summary>
/// プレイヤーの状態作成
/// </summary>
class PlayerStateFactory
{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static PlayerStateFactory* GetInstance();

public: // メンバ関数

	// シーン生成
	IPlayerState* CreatePlayerState(uint32_t playerStateName);

private:
	PlayerStateFactory() = default;
	~PlayerStateFactory() = default;
	PlayerStateFactory(const PlayerStateFactory&) = delete;
	const PlayerStateFactory& operator=(const PlayerStateFactory&) = delete;

};

