#pragma once
#include <cstdint>

/// <summary>
/// 布用バネ
/// </summary>
struct ClothSpring
{
	uint32_t pointIndex0_; // 質点0
	uint32_t pointIndex1_; // 質点1
	float naturalLength_; // 自然長
	uint32_t type_; // バネの種類
};