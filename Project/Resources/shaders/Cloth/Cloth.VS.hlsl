#include "Cloth.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b5);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input) {

	VertexShaderOutput output;
	
	output.position = mul(input.position, gTransformationMatrix.WVP);
	output.texcoord = float32_t2(0.0f, 0.0f);
	output.normal = float32_t3( 0.0f,0.0f,-1.0f );

	return output;

}
