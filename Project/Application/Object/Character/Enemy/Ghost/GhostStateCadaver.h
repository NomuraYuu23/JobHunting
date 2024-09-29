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

private:

	// 透明度
	float alpha = 1.0f;

	// 透明度変更速度
	float alphaSpeed = 0.01f;

	// 透明度変更が始まる時間
	float alphaChangeStartTime = 10.0f;

	// 透明度変更が始まる時間カウント
	float alphaChangeStartTimeCount = 0.0f;

};

