#include "GPUParticle.CS.hlsli"

struct Emitter {
	float32_t3 translate; // 位置
	float32_t radius; // 射出半径
	uint32_t count; // 射出数
	float32_t frequency; // 射出間隔
	float32_t frequencyTime; // 射出間隔調整時間
	uint32_t emit; // 射出許可
};

ConstantBuffer<Emitter> gEmitter : register(b0);

RWStructuredBuffer<Particle> gParticles : register(u0);

[numthreads(1, 1, 1)]
void main( uint32_t3 DTid : SV_DispatchThreadID )
{

	// 射出許可がでた
	if (gEmitter.emit != 0) {
		for (uint32_t conutIndex = 0; conutIndex < gEmitter.count; ++conutIndex) {
			// カウント分
			gParticles[conutIndex].scale = float32_t3(0.3f, 0.3f, 0.3f);
			gParticles[conutIndex].translate = float32_t3(0.0f, 0.0f, 0.0f);
			gParticles[conutIndex].color = float32_t4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

}