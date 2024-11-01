#pragma once
#include "../../Math/Vector/Vector3.h"

/// <summary>
/// 外部操作
/// </summary>
struct ClothExternalOperationData
{
	Vector3 position_; // 位置
	uint32_t isMove_; // 位置動かすか
	float weight_; // 重み
};