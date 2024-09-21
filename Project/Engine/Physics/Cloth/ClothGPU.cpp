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

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE massPointDescriptorRange[1] = {};
	massPointDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	massPointDescriptorRange[0].NumDescriptors = 1;//数は一つ
	massPointDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//UAVを使う
	massPointDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[2].Descriptor.ShaderRegister = 1;//レジスタ番号indexとバインド

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = massPointDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(massPointDescriptorRange);//Tableで利用する数

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = _countof(rootParameters); //配列の長さ

	// サンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].MipLODBias = 0.0f;
	samplerDesc[0].MaxAnisotropy = 0;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].MinLOD = 0.0f;
	samplerDesc[0].MaxLOD = 3.402823466e+38f;
	samplerDesc[0].RegisterSpace = 0;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	descriptionRootsignature.pStaticSamplers = samplerDesc;
	descriptionRootsignature.NumStaticSamplers = _countof(samplerDesc);

	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexInitVertex]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/ClothGPU/ClothInitVertex.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexInitVertex].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexInitVertex]));
	assert(SUCCEEDED(hr));

}

void ClothGPU::PipelineStateCSInitializeForInitMassPoint(ID3D12Device* device)
{

}

void ClothGPU::PipelineStateCSInitializeForInitSpring(ID3D12Device* device)
{
}

void ClothGPU::PipelineStateCSInitializeForInitSurface(ID3D12Device* device)
{


	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE massPointDescriptorRange[1] = {};
	massPointDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	massPointDescriptorRange[0].NumDescriptors = 1;//数は一つ
	massPointDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//UAVを使う
	massPointDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[2].Descriptor.ShaderRegister = 1;//レジスタ番号indexとバインド

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = massPointDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(massPointDescriptorRange);//Tableで利用する数

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = _countof(rootParameters); //配列の長さ

	// サンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].MipLODBias = 0.0f;
	samplerDesc[0].MaxAnisotropy = 0;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].MinLOD = 0.0f;
	samplerDesc[0].MaxLOD = 3.402823466e+38f;
	samplerDesc[0].RegisterSpace = 0;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	descriptionRootsignature.pStaticSamplers = samplerDesc;
	descriptionRootsignature.NumStaticSamplers = _countof(samplerDesc);

	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexInitSurface]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/ClothGPU/ClothInitSurface.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexInitSurface].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexInitSurface]));
	assert(SUCCEEDED(hr));

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

	NumInitialize(device, div);

	MaterialInitialize();

	BufferInitialize(device, commandList, scale, div);

}

void ClothGPU::NumInitialize(ID3D12Device* device, const Vector2& div)
{

	// 頂点数(CPUのIndexと同じ数)
	NumsBuff_ = BufferResource::CreateBufferResource(device, (sizeof(Nums) + 0xff) & ~0xff);
	NumsBuff_->Map(0, nullptr, reinterpret_cast<void**>(&NumsMap_));
	NumsMap_->vertexNum_ = static_cast<uint32_t>(div.y) * static_cast<uint32_t>(div.x) * 6;

	// 質点数
	NumsMap_->massPointNum_ = (static_cast<uint32_t>(div.y) + 1) * (static_cast<uint32_t>(div.x) + 1);

	// バネ数
	NumsMap_->springNum_ = NumsMap_->massPointNum_ * 6;

	// 面数
	NumsMap_->surfaceNum_ = NumsMap_->vertexNum_ / 6;

	// バネフェーズの反復回数
	relaxation_ = 4;

}

void ClothGPU::Update(ID3D12GraphicsCommandList* commandList)
{

	InitVertexCS(commandList);

	InitSurfaceCS(commandList);

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

	CBVInitialize(device, scale, div);

	SRVInitialize(device);

	VertexBufferInitialize(device, commandList);

	UAVInitialize(device, commandList);

}

void ClothGPU::VertexBufferInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

	// UAVデータ
	vertBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(VertexData) + 0xff) & ~0xff) * NumsMap_->vertexNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = NumsMap_->vertexNum_;
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
	srvDesc.Buffer.NumElements = NumsMap_->vertexNum_;
	srvDesc.Buffer.StructureByteStride = sizeof(VertexData);

	vertSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	vertSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	vertSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(vertBuff_.Get(), &srvDesc, vertSrvHandleCPU_);

	InitVertexCS(commandList);

}

void ClothGPU::UAVInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

	// 面情報
	surfaceDataBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(SurfaceData) + 0xff) & ~0xff) * NumsMap_->surfaceNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC surfaceDataUavDesc{};

	surfaceDataUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	surfaceDataUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	surfaceDataUavDesc.Buffer.FirstElement = 0;
	surfaceDataUavDesc.Buffer.NumElements = NumsMap_->surfaceNum_;
	surfaceDataUavDesc.Buffer.CounterOffsetInBytes = 0;
	surfaceDataUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	surfaceDataUavDesc.Buffer.StructureByteStride = sizeof(SurfaceData);

	surfaceDataUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	surfaceDataUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	surfaceDataUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(surfaceDataBuff_.Get(), nullptr, &surfaceDataUavDesc, surfaceDataUavHandleCPU_);

	InitSurfaceCS(commandList);

	// 質点情報
	massPointBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(ClothMassPoint) + 0xff) & ~0xff) * NumsMap_->massPointNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC massPointUavDesc{};

	massPointUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	massPointUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	massPointUavDesc.Buffer.FirstElement = 0;
	massPointUavDesc.Buffer.NumElements = NumsMap_->massPointNum_;
	massPointUavDesc.Buffer.CounterOffsetInBytes = 0;
	massPointUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	massPointUavDesc.Buffer.StructureByteStride = sizeof(ClothMassPoint);

	massPointUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	massPointUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	massPointUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(massPointBuff_.Get(), nullptr, &massPointUavDesc, massPointUavHandleCPU_);



	// バネ情報
	springBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(ClothSpring) + 0xff) & ~0xff) * NumsMap_->springNum_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC springUavDesc{};

	springUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	springUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	springUavDesc.Buffer.FirstElement = 0;
	springUavDesc.Buffer.NumElements = NumsMap_->springNum_;
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
	externalBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(ExternalOperationData) + 0xff) & ~0xff) * NumsMap_->massPointNum_);
	//書き込むためのアドレスを取得
	externalBuff_->Map(0, nullptr, reinterpret_cast<void**>(&externalMap_));

	for (uint32_t i = 0; i < NumsMap_->massPointNum_; ++i) {
		externalMap_[i].position_ = {0.0f,0.0f,0.0f};
		externalMap_[i].wight_ = 1.0f;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC externalSrvDesc{};
	externalSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	externalSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	externalSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	externalSrvDesc.Buffer.FirstElement = 0;
	externalSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	externalSrvDesc.Buffer.NumElements = NumsMap_->massPointNum_;
	externalSrvDesc.Buffer.StructureByteStride = sizeof(ExternalOperationData);

	externalSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	externalSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	externalSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(externalBuff_.Get(), &externalSrvDesc, externalSrvHandleCPU_);

	// 頂点 がどこの質点か
	massPointIndexBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(uint32_t) + 0xff) & ~0xff) * NumsMap_->vertexNum_);
	//書き込むためのアドレスを取得
	massPointIndexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&massPointIndexMap_));

	uint32_t y = 0;
	uint32_t x = 0;
	for (uint32_t i = 0; i < NumsMap_->vertexNum_; ++i) {
		uint32_t mod = i % 6;
		switch (mod)
		{
		case 0:
			massPointIndexMap_[i] = y * (static_cast<uint32_t>(createDataMap_->div_.x) + 1) + x;
			break;
		case 1:
			massPointIndexMap_[i] = y * (static_cast<uint32_t>(createDataMap_->div_.x) + 1) + x + 1;
			break;
		case 2:
			massPointIndexMap_[i] = (y + 1) * (static_cast<uint32_t>(createDataMap_->div_.x) + 1) + x;
			break;
		case 3:
			massPointIndexMap_[i] = y * (static_cast<uint32_t>(createDataMap_->div_.x) + 1) + x + 1;
			break;
		case 4:
			massPointIndexMap_[i] = (y + 1) * (static_cast<uint32_t>(createDataMap_->div_.x) + 1) + x;
			break;
		case 5:
			massPointIndexMap_[i] = (y + 1) * (static_cast<uint32_t>(createDataMap_->div_.x) + 1) + x + 1;
			x++;
			if (x >= (createDataMap_->div_.x)) {
				x = 0;
				y++;
			}
			break;
		default:
			assert(0);
			break;
		}

	}

	D3D12_SHADER_RESOURCE_VIEW_DESC massPointIndexSrvDesc{};
	massPointIndexSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	massPointIndexSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	massPointIndexSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	massPointIndexSrvDesc.Buffer.FirstElement = 0;
	massPointIndexSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	massPointIndexSrvDesc.Buffer.NumElements = NumsMap_->vertexNum_;
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
	clothCalcDataMap_->mass_ = 1.0f;// 質点の質量
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

void ClothGPU::InitVertexCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitVertex].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitVertex].Get());

	commandList->SetComputeRootDescriptorTable(0, vertUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, NumsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, createDataBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, massPointIndexSrvHandleGPU_);

	commandList->Dispatch((NumsMap_->vertexNum_ + 1023) / 1024, 1, 1);

}

void ClothGPU::InitSurfaceCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitSurface].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitSurface].Get());

	commandList->SetComputeRootDescriptorTable(0, surfaceDataUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, NumsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, createDataBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, massPointIndexSrvHandleGPU_);

	commandList->Dispatch((NumsMap_->surfaceNum_ + 1023) / 1024, 1, 1);

}