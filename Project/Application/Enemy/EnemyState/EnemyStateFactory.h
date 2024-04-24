#pragma once
#include "IEnemyState.h"

class EnemyStateFactory
{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static EnemyStateFactory* GetInstance();

public: // メンバ関数

	// シーン生成
	IEnemyState* CreateEnemyState(uint32_t enemyStateName);

private:
	EnemyStateFactory() = default;
	~EnemyStateFactory() = default;
	EnemyStateFactory(const EnemyStateFactory&) = delete;
	const EnemyStateFactory& operator=(const EnemyStateFactory&) = delete;

};

