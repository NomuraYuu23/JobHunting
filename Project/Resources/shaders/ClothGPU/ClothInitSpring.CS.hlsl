#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<ClothSpring> gClothSprings : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<CreateData> gCreateData : register(b1);

RWStructuredBuffer<int32_t> gNextSpringIndex : register(u1);

void SpringGeneration(
	uint32_t x,
	uint32_t y,
	uint32_t offsetX,
	uint32_t offsetY,
	uint32_t type)
{

	int32_t targetX = x + offsetX;
	int32_t targetY = y + offsetY;

	if (targetX >= 0 &&
		targetX < int32_t(gCreateData.div_.x) + 1 &&
		targetY >= 0 &&
		targetY < int32_t(gCreateData.div_.y) + 1) {

		int32_t index;
		InterlockedAdd(gNextSpringIndex[0], -1, index);

		if (0 <= index && index < gNums.springNum_) {

			gClothSprings[index].pointIndex0_ = y * (int32_t(gCreateData.div_.x) + 1) + x;
			gClothSprings[index].pointIndex1_ = targetY * (int32_t(gCreateData.div_.x) + 1) + targetX;

			gClothSprings[index].naturalLength_ = 1.0f;
			gClothSprings[index].type_ = type;
		
		}else{
			InterlockedAdd(gNextSpringIndex[0], 1);
		}

	}

}

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.massPointNum_ > index) {

		uint32_t y = index / uint32_t(gCreateData.div_.x + 1);
		uint32_t x = index % uint32_t(gCreateData.div_.x + 1);


		SpringGeneration(x, y, -1, 0, 0); 
		SpringGeneration(x, y, 0, -1, 0); 

		SpringGeneration(x, y, -1, -1, 1); 
		SpringGeneration(x, y, 1, -1, 1); 

		SpringGeneration(x, y, -2, 0, 2);
		SpringGeneration(x, y, 0, -2, 2);

	}


}