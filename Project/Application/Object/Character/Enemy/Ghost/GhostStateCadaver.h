#pragma once
#include "IGhostState.h"
#include "../../../../../Engine/Physics/StructuralSpring.h"
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

	//バネ
	std::vector<StructuralSpring> structuralSpring_;

	// 剛性。バネ定数k
	float stiffness_ = 750.0f;

	// 減衰係数
	float dampingCoefficient_ = 2.0f;

	// 質量(質点)
	float mass_ = 0.5f;
};

