#pragma once
#include "IAxSpearManState.h"
class AxSpearManStateRoot : public IAxSpearManState
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

