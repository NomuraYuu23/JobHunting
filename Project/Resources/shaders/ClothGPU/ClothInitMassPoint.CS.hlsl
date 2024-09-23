#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<CreateData> gCreateData : register(b1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.massPointNum_ > index) {

		gClothMassPoints[index].y_ = index / uint32_t(gCreateData.div_.x + 1);
		gClothMassPoints[index].x_ = index % uint32_t(gCreateData.div_.x + 1);

		gClothMassPoints[index].position_ = 
			float32_t3(
				gClothMassPoints[index].x_ / gCreateData.div_.x,
				gClothMassPoints[index].y_ / gCreateData.div_.y * -1.0f,
					0.0f);

		gClothMassPoints[index].position_.x *= gCreateData.scale_.x;
		gClothMassPoints[index].position_.y *= gCreateData.scale_.y;

		gClothMassPoints[index].prePosition_ = gClothMassPoints[index].position_;

		if (gClothMassPoints[index].y_ == 0) {
			gClothMassPoints[index].weight_ = 0.0f;
		}
		else {
			gClothMassPoints[index].weight_ = 1.0f;
		}

	}

}