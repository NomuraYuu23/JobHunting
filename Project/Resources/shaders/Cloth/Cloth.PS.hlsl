#include "Cloth.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
	float32_t shininess;
	float32_t environmentCoefficient;
};

SamplerState gSampler : register(s0);

Texture2D<float32_t4> gTexture0 : register(t0);

ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {

	PixelShaderOutput output;

	float32_t4 textureColor = gTexture0.Sample(gSampler, input.texcoord);

	output.color = textureColor * gMaterial.color;

	return output;

}