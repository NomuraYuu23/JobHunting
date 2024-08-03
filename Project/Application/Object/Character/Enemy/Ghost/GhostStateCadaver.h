#pragma once
#include "IGhostState.h"
class GhostStateCadaver :
    public IGhostState
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

private: // メンバ変数

	/// <summary>
	/// 剛体更新
	/// </summary>
	void RigidBodyUpdate();

};

