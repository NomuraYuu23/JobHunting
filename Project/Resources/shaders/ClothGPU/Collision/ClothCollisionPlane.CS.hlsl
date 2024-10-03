#include "../ClothGPU.CS.hlsli"

struct Plane{
	//法線
	float32_t3 normal_;
	//距離
	float32_t distance_;
};

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<Plane> gPlane : register(b1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.massPointNum_ > index) {

		ClothMassPoint massPoint = gClothMassPoints[index];

		// 固定点なら終了
		if (massPoint.weight_ == 0.0f) {
			return;
		}

		// 線情報
		float32_t3 origin = massPoint.prePosition_;
		float32_t3 diff = massPoint.position_ - origin;

		// 内積
		float32_t pDotL = dot(gPlane.normal_, diff);

		// 垂直または向き合ってないなら衝突しない
		if (pDotL >= 0.0f) {
			return;
		}

		// tを求める
		float32_t t = (gPlane.distance_ - dot(origin, gPlane.normal_)) / pDotL;

		// tが1.0f以内でないなら衝突してない
		if (t > 1.0f) {
			return;
		}

		// 押し出し
		float32_t3 extrusion = massPoint.prePosition_ + (diff * t);
		gClothMassPoints[index].position_ = extrusion;

	}

}