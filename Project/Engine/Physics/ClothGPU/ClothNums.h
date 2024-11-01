#pragma once
#include <cstdint>
#include <array>

/// <summary>
/// 数
/// </summary>
struct ClothNums
{
	std::array<uint32_t, 4> structuralSpringNums_; // 構成バネ、4つバッファがある
	std::array<uint32_t, 4> shearSpringNums_; // せん断バネ、4つバッファがある
	std::array<uint32_t, 4> bendingSpringNums_; // 曲げバネ、4つバッファがある

	uint32_t vertexNum_; // 頂点数
	uint32_t massPointNum_; // 質点数
	uint32_t surfaceNum_; // 面数
};