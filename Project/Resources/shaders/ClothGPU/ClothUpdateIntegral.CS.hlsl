#include "ClothGPU.CS.hlsli"
#include "../RandomGenerator/RandomGenerator.hlsli"

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<PerFrame> gPerFrame : register(b1);

ConstantBuffer<ClothCalcData> gClothCalcData : register(b2);

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.massPointNum_ > index) {

		ClothMassPoint massPoint = gClothMassPoints[index];
		// 重力
		float32_t3 force = float32_t3( 0.0f, -9.8f, 0.0f );
		// 風力
		float32_t3 wind = (float32_t3)0;
		force += wind;
		// 変位に変換
		force = force * (gPerFrame.deltaTime * gPerFrame.deltaTime * 0.5f * rcp(gClothCalcData.mass_));

		// 抵抗
		float resistance = 1.0f - gClothCalcData.speedResistance_ * gPerFrame.deltaTime;

		// 変位
		float32_t3 dx = (float32_t3)0;
		// 速度
		dx = massPoint.position_ - massPoint.prePosition_;
		// 前フレーム位置更新
		gClothMassPoints[index].prePosition_ = massPoint.position_;
		// 力の変位を足しこむ
		dx = dx + force + wind;
		// 抵抗
		dx *= resistance;

		// 位置更新
		dx *= massPoint.weight_; // 固定されてるか
		gClothMassPoints[index].position_ += dx;
	
	}

}