#pragma once
#include "../../Math/Vector/Vector3.h"

/// <summary>
/// 面情報
/// </summary>
struct ClothSurfaceData {
	Vector3 normal_; // 法線
	std::array<int32_t, 4> indexes_; // 頂点 四角形固定なので四つ
};
