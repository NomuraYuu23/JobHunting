#include "ClothGPU.h"
#include "../../base/BufferResource.h"
#include "../../base/SRVDescriptorHerpManager.h"
#include "../../base/CompileShader.h"
#include "../../base/Log.h"
#include "../../base/TextureManager.h"
#include "../../Math/DeltaTime.h"


//	平行光源
DirectionalLight* ClothGPU::sDirectionalLight_ = nullptr;
// ポイントライトマネージャ
PointLightManager* ClothGPU::sPointLightManager_ = nullptr;
//	スポットライトマネージャ
SpotLightManager* ClothGPU::sSpotLightManager_ = nullptr;
// 霧マネージャー
FogManager* ClothGPU::sFogManager_ = nullptr;

// ルートシグネチャCS
std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, ClothGPU::kPipelineStateCSIndexOfCount> ClothGPU::rootSignaturesCS_{};
// パイプラインステートオブジェクトCS
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, ClothGPU::kPipelineStateCSIndexOfCount> ClothGPU::pipelineStatesCS_{};

void ClothGPU::StaticInitialize(
	ID3D12Device* device,
	DirectionalLight* sDirectionalLight, 
	PointLightManager* sPointLightManager, 
	SpotLightManager* sSpotLightManager, 
	FogManager* sFogManager)
{

	//	平行光源
	sDirectionalLight_ = sDirectionalLight;
	// ポイントライトマネージャ
	sPointLightManager_ = sPointLightManager;
	//	スポットライトマネージャ
	sSpotLightManager_ = sSpotLightManager;
	// 霧マネージャー
	sFogManager_ = sFogManager;

	PipelineStateCSInitialize(device);

}


void ClothGPU::PipelineStateCSInitialize(ID3D12Device* device)
{

	PipelineStateCSInitializeForInitVertex(device);
	PipelineStateCSInitializeForInitMassPoint(device);
	PipelineStateCSInitializeForInitSpring(device);
	PipelineStateCSInitializeForInitSurface(device);
	PipelineStateCSInitializeForUpdateExternalOperation(device);
	PipelineStateCSInitializeForUpdateIntegral(device);
	PipelineStateCSInitializeForUpdateSpring(device);
	PipelineStateCSInitializeForUpdateVertex(device);

}

void ClothGPU::PipelineStateCSInitializeForInitVertex(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForInitMassPoint(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForInitSpring(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForInitSurface(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForUpdateExternalOperation(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForUpdateIntegral(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForUpdateSpring(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForUpdateVertex(ID3D12Device* device)
{
}

void ClothGPU::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const Vector2& scale, const Vector2& div)
{

	NumInitialize(div);

	MaterialInitialize();

	BufferInitialize(device, commandList, scale, div);

}

void ClothGPU::NumInitialize(const Vector2& div)
{
}

void ClothGPU::MaterialInitialize()
{

	// テクスチャハンドル
	textureHandle_ = TextureManager::Load("Resources/default/clothDemo.png", DirectXCommon::GetInstance());

	// マテリアル
	material_.reset(Material::Create());
	material_->SetEnableLighting(HalfLambert);

}

void ClothGPU::BufferInitialize(ID3D12Device* device,
	ID3D12GraphicsCommandList* commandList,
	const Vector2& scale,
	const Vector2& div)
{

	VertexBufferInitialize(device, commandList);

	UAVInitialize(device, commandList);

	SRVInitialize(device);

	CBVInitialize(device, scale, div);

}

void ClothGPU::VertexBufferInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

	// UAVデータ
	vertBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(VertexData) + 0xff) & ~0xff) * vertexNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = vertexNum_;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(VertexData);

	vertUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	vertUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	vertUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(vertBuff_.Get(), nullptr, &uavDesc, vertUavHandleCPU_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = vertexNum_;
	srvDesc.Buffer.StructureByteStride = sizeof(VertexData);

	vertSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	vertSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	vertSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(vertBuff_.Get(), &srvDesc, vertSrvHandleCPU_);



}

void ClothGPU::UAVInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

	// 面情報
	surfaceDataBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(SurfaceData) + 0xff) & ~0xff) * surfaceNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC surfaceDataUavDesc{};

	surfaceDataUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	surfaceDataUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	surfaceDataUavDesc.Buffer.FirstElement = 0;
	surfaceDataUavDesc.Buffer.NumElements = surfaceNum_;
	surfaceDataUavDesc.Buffer.CounterOffsetInBytes = 0;
	surfaceDataUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	surfaceDataUavDesc.Buffer.StructureByteStride = sizeof(SurfaceData);

	surfaceDataUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	surfaceDataUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	surfaceDataUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(surfaceDataBuff_.Get(), nullptr, &surfaceDataUavDesc, surfaceDataUavHandleCPU_);



	// 質点情報
	massPointBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(ClothMassPoint) + 0xff) & ~0xff) * massPointNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC massPointUavDesc{};

	massPointUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	massPointUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	massPointUavDesc.Buffer.FirstElement = 0;
	massPointUavDesc.Buffer.NumElements = massPointNum_;
	massPointUavDesc.Buffer.CounterOffsetInBytes = 0;
	massPointUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	massPointUavDesc.Buffer.StructureByteStride = sizeof(ClothMassPoint);

	massPointUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	massPointUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	massPointUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(massPointBuff_.Get(), nullptr, &massPointUavDesc, massPointUavHandleCPU_);



	// バネ情報
	springBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(ClothSpring) + 0xff) & ~0xff) * springNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC springUavDesc{};

	springUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	springUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	springUavDesc.Buffer.FirstElement = 0;
	springUavDesc.Buffer.NumElements = springNum_;
	springUavDesc.Buffer.CounterOffsetInBytes = 0;
	springUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	springUavDesc.Buffer.StructureByteStride = sizeof(ClothSpring);

	springUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	springUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	springUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(springBuff_.Get(), nullptr, &springUavDesc, springUavHandleCPU_);



}

void ClothGPU::SRVInitialize(ID3D12Device* device)
{

	// 外部操作
	externalBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(ExternalOperationData) + 0xff) & ~0xff) * massPointNum_);
	//書き込むためのアドレスを取得
	externalBuff_->Map(0, nullptr, reinterpret_cast<void**>(&externalMap_));

	for (uint32_t i = 0; i < massPointNum_; ++i) {
		externalMap_[i].position_ = {0.0f,0.0f,0.0f};
		externalMap_[i].wight_ = 1.0f;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC externalSrvDesc{};
	externalSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	externalSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	externalSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	externalSrvDesc.Buffer.FirstElement = 0;
	externalSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	externalSrvDesc.Buffer.NumElements = massPointNum_;
	externalSrvDesc.Buffer.StructureByteStride = sizeof(ExternalOperationData);

	externalSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	externalSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	externalSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(externalBuff_.Get(), &externalSrvDesc, externalSrvHandleCPU_);

	// 頂点 がどこの質点か
	massPointIndexBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(uint32_t) + 0xff) & ~0xff) * vertexNum_);
	//書き込むためのアドレスを取得
	massPointIndexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&massPointIndexMap_));

	for (uint32_t i = 0; i < vertexNum_; ++i) {
		massPointIndexMap_[i] = 0;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC massPointIndexSrvDesc{};
	massPointIndexSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	massPointIndexSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	massPointIndexSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	massPointIndexSrvDesc.Buffer.FirstElement = 0;
	massPointIndexSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	massPointIndexSrvDesc.Buffer.NumElements = vertexNum_;
	massPointIndexSrvDesc.Buffer.StructureByteStride = sizeof(uint32_t);

	massPointIndexSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	massPointIndexSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	massPointIndexSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(massPointIndexBuff_.Get(), &massPointIndexSrvDesc, massPointIndexSrvHandleCPU_);

}

void ClothGPU::CBVInitialize(
	ID3D12Device* device,
	const Vector2& scale,
	const Vector2& div)
{

	// 作成時データバッファ
	createDataBuff_ = BufferResource::CreateBufferResource(device, (sizeof(CreateData) + 0xff) & ~0xff);
	createDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&createDataMap_));
	//WVPマップ
	createDataMap_->scale_ = scale;
	createDataMap_->div_ = div;

	// WVPバッファ
	wvpBuff_ = BufferResource::CreateBufferResource(device, (sizeof(WVP) + 0xff) & ~0xff);
	wvpBuff_->Map(0, nullptr, reinterpret_cast<void**>(&wvpMap_));
	//WVPマップ
	wvpMap_->matrix_ = Matrix4x4::MakeIdentity4x4();

	// 布計算バッファ
	clothCalcDataBuff_ = BufferResource::CreateBufferResource(device, (sizeof(ClothCalcData) + 0xff) & ~0xff);
	clothCalcDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&clothCalcDataMap_));
	//WVPマップ
	clothCalcDataMap_->mass = 1.0f;// 質点の質量
	clothCalcDataMap_->stiffness_ = 100.0f; // 剛性。バネ定数k
	clothCalcDataMap_->speedResistance_ = 0.2f; // 速度抵抗
	clothCalcDataMap_->structuralShrink_ = 10.0f; // 構成バネ伸び抵抗
	clothCalcDataMap_->structuralStretch_ = 10.0f; // 構成バネ縮み抵抗
	clothCalcDataMap_->shearShrink_ = 10.0f; // せん断バネ伸び抵抗
	clothCalcDataMap_->shearStretch_ = 10.0f; // せん断バネ縮み抵抗
	clothCalcDataMap_->bendingShrink_ = 10.0f; // 曲げバネ伸び抵抗
	clothCalcDataMap_->bendingStretch_ = 10.0f; // 曲げバネ縮み抵抗

	// 時間バッファ
	perFrameBuff_ = BufferResource::CreateBufferResource(device, (sizeof(PerFrame) + 0xff) & ~0xff);
	perFrameBuff_->Map(0, nullptr, reinterpret_cast<void**>(&perFrameMap_));
	// 時間マップ
	perFrameMap_->deltaTime_ = kDeltaTime_;
	perFrameMap_->time_ = 0.0f;

}
