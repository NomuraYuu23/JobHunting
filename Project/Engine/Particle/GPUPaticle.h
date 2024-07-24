#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>
#include "../3D/Model.h"
#include "GPUPerticleView.h"
#include "EmitterCS.h"
#include "../Math/PerFrame.h"

#pragma comment(lib, "dxcompiler.lib")

class GPUPaticle
{

public:


	/// <summary>
	/// CSのパイプラインステート
	/// </summary>
	enum PipelineStateCSIndex {
		kPiprlineStateCSIndexInitialize, // 初期化
		kPiprlineStateCSIndexEmit, // エミット
		kPiprlineStateCSIndexUpdate, // 更新
		kPipelineStateCSIndexOfCount // 数える用
	};


public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static GPUPaticle* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプラインステート</param>
	void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(
		ID3D12GraphicsCommandList* commandList,
		BaseCamera& camera);

private:

	/// <summary>
	/// パイプラインステートの初期化CS
	/// </summary>
	void PipelineStateCSInitialize(ID3D12Device* device);

	/// <summary>
	/// バッファの初期化
	/// </summary>
	/// <param name="device"></param>
	void UAVBufferInitialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// モデルの初期化
	/// </summary>
	void ModelInitialize();

	/// <summary>
	/// 定数バッファ初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void ConstantBufferInitialzie(ID3D12Device* device);

	/// <summary>
	/// GPUParticleViewマッピング
	/// </summary>
	/// <param name="camera">カメラ</param>
	void GPUParticleViewMapping(BaseCamera& camera);

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Emit(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UAVバリア―
	/// </summary>
	/// <param name="commandList"></param>
	void UAVBarrier(ID3D12GraphicsCommandList* commandList);

private: // パイプラインステートの初期化CS

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForInitialize(ID3D12Device* device);

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForEmit(ID3D12Device* device);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForUpdate(ID3D12Device* device);

private:

	// パーティクルの最大数
	const uint32_t kParticleMax = 1024;
	// モデルのディレクトリパス
	const std::string kDirectoryPath = "Resources/Particle/";
	// モデルのファイルの名前
	const std::string kFilename = "plane.obj";

	// UAVバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buff_;

	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE uavHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE uavHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t uavIndexDescriptorHeap_ = 0;

	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t srvIndexDescriptorHeap_ = 0;

	// ルートシグネチャCS
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, kPipelineStateCSIndexOfCount> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kPipelineStateCSIndexOfCount> pipelineStatesCS_;

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature_;
	// パイプラインステートオブジェクト
	ID3D12PipelineState* pipelineState_;

	// モデル
	std::unique_ptr<Model> model_;

	// マテリアル
	std::unique_ptr<Material> material_;

	// GPUParticleViewバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> gpuParticleViewBuff_;

	// GPUParticleViewマップ
	GPUParticleView* gpuParticleViewMap_ = nullptr;

	// エミッタバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> emitterBuff_;

	// エミッタマップ
	EmitterCS* emitterMap_ = nullptr;

	// 時間バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameBuff_;

	// 時間マップ
	PerFrame* perFrameMap_ = nullptr;

	// フリーリストインデックスUAVバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListIndexBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE freeListIndexHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE freeListIndexHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t freeListIndexDescriptorHeap_ = 0;

	// フリーリストUAVバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE freeListHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE freeListHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t freeListDescriptorHeap_ = 0;

private: // シングルトン

	GPUPaticle() = default;
	~GPUPaticle() = default;
	GPUPaticle(const GPUPaticle&) = delete;
	GPUPaticle& operator=(const GPUPaticle&) = delete;

};

