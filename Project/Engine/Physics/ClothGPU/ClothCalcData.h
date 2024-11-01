#pragma once
#include "../../Math/Vector/Vector3.h"

/// <summary>
/// 布計算データ
/// </summary>
struct ClothCalcData
{
	float mass_; // 質点の質量
	Vector3 gravity_; // 重力
	Vector3 wind_; // 風力
	float stiffness_; // 剛性。バネ定数k
	float speedResistance_; // 速度抵抗
	float structuralShrink_; // 構成バネ伸び抵抗
	float structuralStretch_; // 構成バネ縮み抵抗
	float shearShrink_; // せん断バネ伸び抵抗
	float shearStretch_; // せん断バネ縮み抵抗
	float bendingShrink_; // 曲げバネ伸び抵抗
	float bendingStretch_; // 曲げバネ縮み抵抗
};