#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<VertexData> gVertexDatas : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

StructuredBuffer<uint32_t> gMassPointIndexes : register(t1);

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u2);

RWStructuredBuffer<SurfaceData> gSurfaceDatas : register(u3);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.vertexNum_ > index) {

		uint32_t massPointIndex = gMassPointIndexes[index];

		//uint32_t xMax = uint32_t(gCreateData.div_.x + 1.0f);

		//uint32_t y = massPointIndex / xMax;
		//uint32_t x = massPointIndex % xMax;

		float32_t3 position = gClothMassPoints[massPointIndex].position_;

		gVertexDatas[index].position_ =
			float32_t4(
				position.x,
				position.y,
				position.z,
				1.0f);

		//gVertexDatas[index].texcoord_ = float32_t2(float32_t(x) * rcp(gCreateData.div_.x), float32_t(y) * rcp(gCreateData.div_.y));

		// 法線マッピング
		float32_t3 normals[4];
		uint32_t normalNum = 0;

		uint32_t j = 0;
		uint32_t k = 0;

		for (j = 0; j < gNums.surfaceNum_; ++j) {
			for (k = 0; k < 4; ++k) {
				if (gSurfaceDatas[j].indexes_[k] == massPointIndex) {
					normals[normalNum] = gSurfaceDatas[j].normal_;
					normalNum++;
					break;
				}
			}

			if (normalNum >= 4) {
				break;
			}

		}

		float32_t3 normal = { 0.0f,0.0f,0.0f };
		for (j = 0; j < normalNum; ++j) {
			normal = normals[j];
		}

		gVertexDatas[index].normal_ = normalize(normal);

	}

}