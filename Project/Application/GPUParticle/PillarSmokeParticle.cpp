#include "PillarSmokeParticle.h"
#include "../../Engine/base/BufferResource.h"
#include "../../Engine/Particle/ParticleCS.h"
#include "../../Engine/base/SRVDescriptorHerpManager.h"
#include "../../Engine/base/CompileShader.h"
#include "../../Engine/base/Log.h"
#include "../../Engine/base/TextureManager.h"
#include "../../Engine/Particle/BillBoardMatrix.h"
#include "../../Engine/Math/DeltaTime.h"

void PillarSmokeParticle::Initialize(
	ID3D12Device* device,
	ID3D12GraphicsCommandList* commandList,
	ID3D12RootSignature* rootSignature,
	ID3D12PipelineState* pipelineState)
{

	textureFilename_ = "Bonfire.png";

	GPUParticle::Initialize(device, commandList, rootSignature, pipelineState);

	EmitterCS emitter;
	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.frequencyTime = 0.0f;
	emitter.translate = Vector3(0.0f, 3.0f, 0.0f);
	emitter.radius = 1.0f;
	emitter.emit = 0;


	SetEmitter(emitter);

}

void PillarSmokeParticle::Draw(ID3D12GraphicsCommandList* commandList, BaseCamera& camera)
{

	assert(commandList);

	// GPUParticleViewのマッピング
	GPUParticleViewMapping(camera);

	// エミット
	Emit(commandList);

	// バリア
	UAVBarrier(commandList);

	// 更新
	UpdateCS(commandList);

	// リソースバリア
	ResouseBarrierToNonPixelShader(commandList);

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//形状を設定。PS0に設定しているものとは別。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// パイプライン
	commandList->SetPipelineState(pipelineState_);
	// ルートシグネチャ
	commandList->SetGraphicsRootSignature(rootSignature_);

	// 頂点データ
	commandList->IASetVertexBuffers(0, 1, model_->GetMesh()->GetVbView());

	// GPUパーティクル用
	commandList->SetGraphicsRootDescriptorTable(0, srvHandleGPU_);
	// GPUパーティクルのView
	commandList->SetGraphicsRootConstantBufferView(1, gpuParticleViewBuff_->GetGPUVirtualAddress());
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		commandList,
		2,
		textureHandle_);
	// マテリアル
	commandList->SetGraphicsRootConstantBufferView(3, material_->GetMaterialBuff()->GetGPUVirtualAddress());
	// Dissolve
	commandList->SetGraphicsRootDescriptorTable(4, srvDissolveHandleGPU_);
	// 描画
	commandList->DrawInstanced(6, kParticleMax, 0, 0);

	// リソースバリア
	ResouseBarrierToUnorderedAccess(commandList);

}

void PillarSmokeParticle::UAVBufferInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

	// バッファ
	dissolveBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(float) + 0xff) & ~0xff) * kParticleMax);

	/// UAV

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = kParticleMax;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(float);

	uavDissolveHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	uavDissolveHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	uavDissolveDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(dissolveBuff_.Get(), nullptr, &uavDesc, uavDissolveHandleCPU_);

	/// ここまでUAV

	/// SRV

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kParticleMax;
	srvDesc.Buffer.StructureByteStride = sizeof(float);

	srvDissolveHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	srvDissolveHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	srvDissolveDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(dissolveBuff_.Get(), &srvDesc, srvDissolveHandleCPU_);

	/// ここまでSRV

	GPUParticle::UAVBufferInitialize(device, commandList);

}

void PillarSmokeParticle::InitialzieCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitialize].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitialize].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootDescriptorTable(1, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(2, freeListHandleGPU_);

	commandList->SetComputeRootDescriptorTable(3, uavDissolveHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void PillarSmokeParticle::Emit(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexEmit].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexEmit].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, emitterBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(4, freeListHandleGPU_);

	commandList->SetComputeRootDescriptorTable(5, uavDissolveHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void PillarSmokeParticle::UpdateCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdate].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdate].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(2, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(3, freeListHandleGPU_);

	commandList->SetComputeRootDescriptorTable(4, uavDissolveHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void PillarSmokeParticle::ResouseBarrierToNonPixelShader(ID3D12GraphicsCommandList* commandList)
{


	GPUParticle::ResouseBarrierToNonPixelShader(commandList);

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dissolveBuff_.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	commandList->ResourceBarrier(1, &barrier);

}

void PillarSmokeParticle::ResouseBarrierToUnorderedAccess(ID3D12GraphicsCommandList* commandList)
{

	GPUParticle::ResouseBarrierToUnorderedAccess(commandList);

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dissolveBuff_.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	commandList->ResourceBarrier(1, &barrier);

}

void PillarSmokeParticle::PipelineStateCSInitializeForInitialize(ID3D12Device* device)
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

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE dissolveDescriptorRange[1] = {};
	dissolveDescriptorRange[0].BaseShaderRegister = 3;//iから始まる
	dissolveDescriptorRange[0].NumDescriptors = 1;//数は一つ
	dissolveDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	dissolveDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = dissolveDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(dissolveDescriptorRange);//Tableで利用する数

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexInitialize]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/PillarSmokeParticle/PillarSmokeInitialize.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexInitialize].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexInitialize]));
	assert(SUCCEEDED(hr));

}

void PillarSmokeParticle::PipelineStateCSInitializeForEmit(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[6] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE dissolveDescriptorRange[1] = {};
	dissolveDescriptorRange[0].BaseShaderRegister = 3;//iから始まる
	dissolveDescriptorRange[0].NumDescriptors = 1;//数は一つ
	dissolveDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	dissolveDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

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
	rootParameters[3].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[5].DescriptorTable.pDescriptorRanges = dissolveDescriptorRange;//Tableの中身の配列を指定
	rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(dissolveDescriptorRange);//Tableで利用する数

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexEmit]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/PillarSmokeParticle/PillarSmokeEmit.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexEmit].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexEmit]));
	assert(SUCCEEDED(hr));

}

void PillarSmokeParticle::PipelineStateCSInitializeForUpdate(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE dissolveDescriptorRange[1] = {};
	dissolveDescriptorRange[0].BaseShaderRegister = 3;//iから始まる
	dissolveDescriptorRange[0].NumDescriptors = 1;//数は一つ
	dissolveDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	dissolveDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = dissolveDescriptorRange;//Tableの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(dissolveDescriptorRange);//Tableで利用する数

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexUpdate]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/PillarSmokeParticle/PillarSmokeUpdate.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexUpdate].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexUpdate]));
	assert(SUCCEEDED(hr));

}
