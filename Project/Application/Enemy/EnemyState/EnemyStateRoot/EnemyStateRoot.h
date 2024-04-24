#pragma once
#include "../IEnemyState.h"
class EnemyStateRoot : public IEnemyState
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

};

