#pragma once
#include "../../Math/Vector4.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector2.h"
#include "../../Math/Matrix4x4.h"

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>
#include "../../Math/PerFrame.h"
#include "../../3D/Material.h"

#pragma comment(lib, "dxcompiler.lib")

class ClothGPU
{

public: // サブクラス

	/// <summary>
	/// 頂点
	/// </summary>
	struct VertexData {
		Vector4 position_; // 位置
		Vector2 texcoord_; // テクスチャ座標
		Vector3 normal_; // 法線
	};

	/// <summary>
	/// WVP
	/// </summary>
	struct WVP {
		Matrix4x4 matrix_; // 行列
	};

	/// <summary>
	/// 面情報
	/// </summary>
	struct SurfaceData {
		Vector3 normal_; // 法線
		std::array<int32_t, 4> indexes_; // 頂点
	};

	/// <summary>
	/// 作成情報
	/// </summary>
	struct CreateData
	{
		Vector2 scale_; // 大きさ
		Vector2 div_; // 分割数
	};

	/// <summary>
	/// 布計算データ
	/// </summary>
	struct ClothCalcData
	{
		float mass = 1.0f; // 質点の質量
		float stiffness_; // 剛性。バネ定数k

		float speedResistance_; // 速度抵抗

		int32_t relaxation_; // バネフェーズの反復回数

		float structuralShrink_; // 構成バネ伸び抵抗
		float structuralStretch_; // 構成バネ縮み抵抗
		float shearShrink_; // せん断バネ伸び抵抗
		float shearStretch_; // せん断バネ縮み抵抗
		float bendingShrink_; // 曲げバネ伸び抵抗
		float bendingStretch_; // 曲げバネ縮み抵抗
	};

	/// <summary>
	/// 外部操作
	/// </summary>
	struct ExternalOperationData
	{	
		Vector3 position_; // 位置
		float wight_; // 重み
	};

	/// <summary>
	/// 布用質点
	/// </summary>
	struct ClothMassPoint
	{
		Vector3 position_; // 現在位置
		Vector3 prePosition_; // 前フレ―ムの位置
		float weight_; // 運動計算の重み (固定する場合は0.0f, それ以外は1.0f)
		uint32_t y_; // y 検索用
		uint32_t x_; // x 検索用
	};

	/// <summary>
	/// 布用バネ
	/// </summary>
	struct ClothSpring
	{
		uint32_t pointIndex0_; // 質点0
		uint32_t pointIndex1_; // 質点1
		float naturalLength_; // 自然長
		uint32_t type_; // バネの種類
	}; 

	/// <summary>
	/// CSのパイプラインステート
	/// </summary>
	enum PipelineStateCSIndex {
		kPipelineStateCSIndexInitialize, // 初期化
		kPipelineStateCSIndexUpdate, // 更新
		kPipelineStateCSIndexOfCount // 数える用
	};

public:


private:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	virtual void PipelineStateCSInitializeForInitialize(ID3D12Device* device);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="device"></param>
	virtual void PipelineStateCSInitializeForUpdate(ID3D12Device* device);

private: // UAV & SRV

	// 頂点 (頂点の数)
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	//UAV
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE vertUavHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE vertUavHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t vertUavIndexDescriptorHeap_ = 0;
	// SRV
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE vertSrvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE vertSrvHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t vertSrvIndexDescriptorHeap_ = 0;

private: // SRV

	// 外部操作 (質点の数)
	Microsoft::WRL::ComPtr<ID3D12Resource> externalBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE externalSrvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE externalSrvHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t externalSrvIndexDescriptorHeap_ = 0;

private: // CBV

	// 作成時データバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> createDataBuff_;
	//WVPマップ
	CreateData* createDataMap_ = nullptr;

	// WVPバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpBuff_;
	//WVPマップ
	WVP* wvpMap_ = nullptr;

	// 布計算バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> clothCalcDataBuff_;
	//WVPマップ
	ClothCalcData* clothCalcDataMap_ = nullptr;

	// 時間バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameBuff_;
	// 時間マップ
	PerFrame* perFrameMap_ = nullptr;

private: // UAV

	// 面情報 (面の数)
	Microsoft::WRL::ComPtr<ID3D12Resource> surfaceDataBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE surfaceDataUavHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE surfaceDataUavHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t surfaceDataUavIndexDescriptorHeap_ = 0;

	// 質点情報 (質点の数)
	Microsoft::WRL::ComPtr<ID3D12Resource> massPointBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE massPointUavHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE massPointUavHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t massPointUavIndexDescriptorHeap_ = 0;

	// バネ情報 (バネの数)
	Microsoft::WRL::ComPtr<ID3D12Resource> springBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE springUavHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE springUavHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t springUavIndexDescriptorHeap_ = 0;

private: // 変数

	// 数
	uint32_t vertexNum_;
	uint32_t massPointNum_;
	uint32_t springNum_;
	uint32_t surfaceNum_;

	// テクスチャハンドル
	uint32_t textureHandle_;

	// マテリアル
	std::unique_ptr<Material> material_;

private: // CS

	// ルートシグネチャCS
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, kPipelineStateCSIndexOfCount> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kPipelineStateCSIndexOfCount> pipelineStatesCS_;

};

