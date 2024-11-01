#pragma once
#include "../../Math/Vector/Vector3.h"

/// <summary>
/// 布用質点
/// </summary>
struct ClothMassPoint
{
	Vector3 position_; // 現在の位置
	Vector3 prePosition_; // 前フレ―ムの位置
	float weight_; // 運動計算の重み (固定する場合は0.0f, それ以外は1.0f)
};