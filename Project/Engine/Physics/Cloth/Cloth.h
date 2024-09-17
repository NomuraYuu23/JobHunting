#pragma once
#include "../../Math/Vector3.h"
#include "../../Math/Vector2.h"
class Cloth
{

public: // サブクラス

	/// <summary>
	/// バネの種類
	/// </summary>
	enum TypeOfSpring {
		StructuralSpring = 0, // 構成バネ
		ShearSpring = 1, // せん断バネ
		BendingSpring = 2, // 曲げバネ
	};

	/// <summary>
	/// 布用質点
	/// </summary>
	struct ClothMassPoint
	{
		Vector3 position_; // 現在位置
		Vector3 prePosition_; // 前フレ―ムの位置
		float weight_; // 運動計算の重み (固定する場合は0.0f, それ以外は1.0f)
	};
	
	/// <summary>
	/// 布用バネ
	/// </summary>
	struct ClothSpring
	{
		ClothMassPoint* point0_; // 質点0
		ClothMassPoint* point1_; // 質点1
		float naturalLength_; // 自然長
		TypeOfSpring type_; // バネの種類
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scale">大きさ</param>
	/// <param name="div">分割数</param>
	void Initialize(const Vector2& scale, const Vector2& div);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // 初期化関数

	/// <summary>
	/// 質点初期化
	/// </summary>
	void MassPointsInitialize();

	/// <summary>
	/// バネ初期化
	/// </summary>
	void SpringInitialize();

private: // 更新関数

	/// <summary>
	/// 積分フェーズ
	/// </summary>
	void IntegralPhase();

	/// <summary>
	/// バネフェーズ
	/// </summary>
	void SpringPhase();

private: // その他関数

	/// <summary>
	/// バネ作成
	/// </summary>
	/// <param name="x">始点X</param>
	/// <param name="y">始点Y</param>
	/// <param name="offsetX">始点から終点への距離X</param>
	/// <param name="offsetY">始点から終点への距離Y</param>
	/// <param name="type">タイプ</param>
	void SpringGeneration(
		uint32_t x, 
		uint32_t y, 
		uint32_t offsetX, 
		uint32_t offsetY, 
		TypeOfSpring type);

private: // メンバ変数

	Vector2 scale_; // 大きさ
	Vector2 div_; // 分割数
	std::vector<ClothMassPoint> massPoints_; // 質点
	std::vector<ClothSpring> springs_; // バネ

	float mass = 1.0f; // 質点の質量
	float stiffness_; // 剛性。バネ定数k
	float dampingCoefficient_; // 減衰係数

	float speedResistance_; // 速度抵抗

};

