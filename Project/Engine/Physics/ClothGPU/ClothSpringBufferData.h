#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>
#include "../../Math/PerFrame.h"
#include "../../3D/Material/Material.h"
#include "ClothSpring.h"

#pragma comment(lib, "dxcompiler.lib")

class ClothSpringBufferData
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="num">数</param>
	void Initialize(ID3D12Device* device, uint32_t num);

public: // アクセッサ

	/// <summary>
	/// ディスクリプタヒープの位置
	/// </summary>
	/// <returns>ディスクリプタヒープの位置</returns>
	uint32_t GetSrvIndexDescriptorHeap() { return srvIndexDescriptorHeap_; }

	/// <summary>
	/// GPUハンドル
	/// </summary>
	/// <returns>GPUハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU() { return srvHandleGPU_; }

	/// <summary>
	/// 頂点 がどこの質点かマップ
	/// </summary>
	/// <param name="map">データ</param>
	/// <param name="index">番号</param>
	void SetMap(const ClothSpring& map, uint32_t index) { map_[index] = map; }

private: // 変数

	// バネ情報 (バネの数)
	Microsoft::WRL::ComPtr<ID3D12Resource> buff_;
	// バネマップ
	ClothSpring* map_ = nullptr;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t srvIndexDescriptorHeap_ = 0;

};

