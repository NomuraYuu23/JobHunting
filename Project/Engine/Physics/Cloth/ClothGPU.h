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

#include "../../Camera/BaseCamera.h"
#include "../../Light/DirectionalLight/DirectionalLight.h"
#include "../../Light/PointLight/PointLightManager.h"
#include "../../Light/SpotLight/SpotLightManager.h"
#include "../../3D/FogManager.h"

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
		float mass_; // 質点の質量
		float stiffness_; // 剛性。バネ定数k

		float speedResistance_; // 速度抵抗


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
	/// 数
	/// </summary>
	struct Nums
	{
		uint32_t vertexNum_;
		uint32_t massPointNum_;
		uint32_t springNum_;
		uint32_t surfaceNum_;
	};

	/// <summary>
	/// CSのパイプラインステート
	/// </summary>
	enum PipelineStateCSIndex {
		kPipelineStateCSIndexInitVertex,// 初期化 頂点
		kPipelineStateCSIndexInitMassPoint,// 初期化 質点
		kPipelineStateCSIndexInitSpring,// 初期化 バネ
		kPipelineStateCSIndexInitSurface,// 初期化 面

		kPipelineStateCSIndexUpdateExternalOperation,// 更新 外部操作フェーズ
		kPipelineStateCSIndexUpdateIntegral,// 更新 積分フェーズ
		kPipelineStateCSIndexUpdateSpring,// 更新 バネフェーズ
		kPipelineStateCSIndexUpdateVertex,// 更新 頂点フェーズ

		kPipelineStateCSIndexOfCount // 数える用
	};

private: // 静的メンバ変数

	//	平行光源
	static DirectionalLight* sDirectionalLight_;
	// ポイントライトマネージャ
	static PointLightManager* sPointLightManager_;
	//	スポットライトマネージャ
	static SpotLightManager* sSpotLightManager_;
	// 霧マネージャー
	static FogManager* sFogManager_;

	// ルートシグネチャCS
	static std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, kPipelineStateCSIndexOfCount> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kPipelineStateCSIndexOfCount> pipelineStatesCS_;

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sDirectionalLight"></param>
	/// <param name="sPointLightManager"></param>
	/// <param name="sSpotLightManager"></param>
	/// <param name="sFogManager"></param>
	static void StaticInitialize(
		ID3D12Device* device,
		DirectionalLight* sDirectionalLight,
		PointLightManager* sPointLightManager,
		SpotLightManager* sSpotLightManager,
		FogManager* sFogManager);

private: // CSの初期化、設定

	/// <summary>
	/// パイプラインステートの初期化CS
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitialize(ID3D12Device* device);

	/// <summary>
	/// 初期化 頂点
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForInitVertex(ID3D12Device* device);

	/// <summary>
	/// 初期化 質点
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForInitMassPoint(ID3D12Device* device);

	/// <summary>
	/// 初期化 バネ
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForInitSpring(ID3D12Device* device);

	/// <summary>
	/// 初期化 面
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForInitSurface(ID3D12Device* device);

	/// <summary>
	/// 更新 外部操作フェーズ
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForUpdateExternalOperation(ID3D12Device* device);

	/// <summary>
	/// 更新 積分フェーズ
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForUpdateIntegral(ID3D12Device* device);

	/// <summary>
	/// 更新 バネフェーズ
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForUpdateSpring(ID3D12Device* device);

	/// <summary>
	/// 更新 頂点フェーズ
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForUpdateVertex(ID3D12Device* device);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="scale">大きさ</param>
	/// <param name="div">分割数</param>
	void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList, 
		const Vector2& scale, 
		const Vector2& div);

	///// <summary>
	///// 更新
	///// </summary>
	void Update(ID3D12GraphicsCommandList* commandList);

	//void Draw();

private: // 変数の初期化

	/// <summary>
	/// 数の初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="div"></param>
	void NumInitialize(ID3D12Device* device, const Vector2& div);

	/// <summary>
	/// マテリアル初期化
	/// </summary>
	void MaterialInitialize();

private: // バッファの初期化、設定

	/// <summary>
	/// バッファの初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="commandList"></param>
	/// <param name="scale"></param>
	/// <param name="div"></param>
	void BufferInitialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList, 
		const Vector2& scale, 
		const Vector2& div);

	/// <summary>
	///	頂点バッファの初期化
	/// </summary>
	/// <param name="device"></param>
	void VertexBufferInitialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UAVの初期化
	/// </summary>
	/// <param name="device"></param>
	void UAVInitialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// SRVの初期化
	/// </summary>
	/// <param name="device"></param>
	void SRVInitialize(ID3D12Device* device);

	/// <summary>
	/// CBVの初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="scale">大きさ</param>
	/// <param name="div">分割数</param>
	void CBVInitialize(
		ID3D12Device* device,
		const Vector2& scale,
		const Vector2& div);

private: // CS


	/// <summary>
	/// 頂点バッファ初期化
	/// </summary>
	/// <param name="commandList"></param>
	void InitVertexCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 質点バッファ初期化
	/// </summary>
	/// <param name="commandList"></param>
	void InitMassPointCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="commandList"></param>
	void InitSpringCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="commandList"></param>
	void InitSurfaceCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateExternalOperationCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateIntegralCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateSpringCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateVertexCS(ID3D12GraphicsCommandList* commandList);

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
	// 外部操作マップ
	ExternalOperationData* externalMap_ = nullptr;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE externalSrvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE externalSrvHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t externalSrvIndexDescriptorHeap_ = 0;

	// 頂点 がどこの質点か (頂点の数)
	Microsoft::WRL::ComPtr<ID3D12Resource> massPointIndexBuff_;
	// 頂点 がどこの質点かマップ
	uint32_t* massPointIndexMap_ = nullptr;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE massPointIndexSrvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE massPointIndexSrvHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t massPointIndexSrvIndexDescriptorHeap_ = 0;

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

	// 数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> NumsBuff_;
	// 数マップ
	Nums* NumsMap_ = nullptr;

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

	// テクスチャハンドル
	uint32_t textureHandle_;

	// マテリアル
	std::unique_ptr<Material> material_;

	// バネフェーズの反復回数
	int32_t relaxation_;

};

