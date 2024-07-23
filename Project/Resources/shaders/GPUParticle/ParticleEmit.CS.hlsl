#include "GPUParticle.CS.hlsli"
#include "../RandomGenerator/RandomGenerator.hlsli"

struct Emitter {
	float32_t3 translate; // 位置
	float32_t radius; // 射出半径
	uint32_t count; // 射出数
	float32_t frequency; // 射出間隔
	float32_t frequencyTime; // 射出間隔調整時間
	uint32_t emit; // 射出許可
};

ConstantBuffer<Emitter> gEmitter : register(b0);

ConstantBuffer<PerFrame> gPerFrame : register(b1);

RWStructuredBuffer<Particle> gParticles : register(u0);

[numthreads(1, 1, 1)]
void main( uint32_t3 DTid : SV_DispatchThreadID )
{

	// 射出許可がでた
	if (gEmitter.emit != 0) {

		RandomGenerator generator;

		generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;

		for (uint32_t conutIndex = 0; conutIndex < gEmitter.count; ++conutIndex) {
			// カウント分
			gParticles[conutIndex].scale = generator.Generate3d();
			gParticles[conutIndex].translate = generator.Generate3d();
			gParticles[conutIndex].color.rgb = generator.Generate3d();
			gParticles[conutIndex].color.a = 1.0f;
			gParticles[conutIndex].lifeTime = 1.0f;
			gParticles[conutIndex].velocity = generator.Generate3d();
			gParticles[conutIndex].currentTime = 0.0f;
		
		}
	}

}