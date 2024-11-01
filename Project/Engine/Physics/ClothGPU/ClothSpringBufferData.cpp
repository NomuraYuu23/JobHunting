#include "ClothSpringBufferData.h"
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"

void ClothSpringBufferData::Initialize(ID3D12Device* device, uint32_t num)
{

	// 外部操作
	buff_ = BufferResource::CreateBufferResource(device, ((sizeof(ClothSpring) + 0xff) & ~0xff) * num);
	//書き込むためのアドレスを取得
	buff_->Map(0, nullptr, reinterpret_cast<void**>(&map_));

	D3D12_SHADER_RESOURCE_VIEW_DESC externalSrvDesc{};
	externalSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	externalSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	externalSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	externalSrvDesc.Buffer.FirstElement = 0;
	externalSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	externalSrvDesc.Buffer.NumElements = num;
	externalSrvDesc.Buffer.StructureByteStride = sizeof(ClothSpring);

	srvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	srvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	srvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(buff_.Get(), &externalSrvDesc, srvHandleCPU_);

}
