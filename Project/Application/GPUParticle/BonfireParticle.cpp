#include "BonfireParticle.h"

void BonfireParticle::Initialize(
	ID3D12Device* device, 
	ID3D12GraphicsCommandList* commandList, 
	ID3D12RootSignature* rootSignature, 
	ID3D12PipelineState* pipelineState)
{

	textureFilename_ = "Bonfire.png";

	GPUPaticle::Initialize(device, commandList, rootSignature, pipelineState);

}
