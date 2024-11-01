#pragma once
#include "../../Math/Vector/Vector4.h"
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Vector/Vector2.h"
#include "../../Math/Matrix/Matrix4x4.h"

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>
#include "../../Math/PerFrame.h"
#include "../../3D/Material/Material.h"

#pragma comment(lib, "dxcompiler.lib")

#include "../../Camera/BaseCamera.h"
#include "../../Light/DirectionalLight/DirectionalLight.h"
#include "../../Light/PointLight/PointLightManager.h"
#include "../../Light/SpotLight/SpotLightManager.h"
#include "../../3D/Fog/FogManager.h"
#include "ClothGPUCollision.h"
#include "ClothWVP.h"
#include "ClothCreateData.h"
#include "ClothExternalOperationData.h"
#include "ClothCalcData.h"
#include "ClothSpring.h"
#include "ClothNums.h"
#include "ClothSpringBufferData.h"

class ClothGPU
{

public: // サブクラス

	/// <summary>
	/// バネの種類
	/// </summary>
	enum ClothSpringBufferStructIndex {
		kClothSpringBufferStructIndexStructuralLeftOdd, // 構成バネ、左、奇数
		kClothSpringBufferStructIndexStructuralLeftEven, // 構成バネ、左、偶数
		kClothSpringBufferStructIndexStructuralTopOdd, // 構成バネ、上、奇数
		kClothSpringBufferStructIndexStructuralTopEven, // 構成バネ、上、偶数

		kClothSpringBufferStructIndexShearLeftOdd, // せん断バネ、左上、奇数
		kClothSpringBufferStructIndexShearLeftEven, // せん断バネ、左上、偶数
		kClothSpringBufferStructIndexShearRightOdd, // せん断バネ、右上、奇数
		kClothSpringBufferStructIndexShearRightEven, // せん断バネ、右上、偶数

		kClothSpringBufferStructIndexBendingLeftNotPrime, // 曲げバネ、左、(質点index % 4 <= 1)
		kClothSpringBufferStructIndexBendingLeftPrime, // 曲げバネ、左、(質点index % 4 > 1)
		kClothSpringBufferStructIndexBendingTopNotPrime, // 曲げバネ、上、(質点index % 4 <= 1)
		kClothSpringBufferStructIndexBendingTopPrime, // 曲げバネ、上、(質点index % 4 > 1)

		kClothSpringBufferStructIndexOfCount // 数える用
	};

	/// <summary>
	/// CSのパイプラインステート
	/// </summary>
	enum PipelineStateCSIndex {
		kPipelineStateCSIndexInitVertex,// 初期化 頂点
		kPipelineStateCSIndexInitMassPoint,// 初期化 質点
		kPipelineStateCSIndexInitSurface,// 初期化 面

		kPipelineStateCSIndexUpdateExternalOperation,// 更新 外部操作フェーズ
		kPipelineStateCSIndexUpdateIntegral,// 更新 積分フェーズ
		kPipelineStateCSIndexUpdateSpring,// 更新 バネフェーズ
		kPipelineStateCSIndexUpdateSurface,// 更新 面
		kPipelineStateCSIndexUpdateVertex,// 更新 頂点フェーズ

		kPipelineStateCSIndexOfCount // 数える用
	};

	/// <summary>
	/// バネの種類
	/// </summary>
	enum TypeOfSpring {
		kTypeOfSpringStructuralSpring = 0, // 構成バネ
		kTypeOfSpringShearSpring = 1, // せん断バネ
		kTypeOfSpringBendingSpring = 2, // 曲げバネ
		kTypeOfSpringOfCount // 数える用
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
	/// 更新 面
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForUpdateSurface(ID3D12Device* device);

	/// <summary>
	/// 更新 頂点フェーズ
	/// </summary>
	/// <param name="device"></param>
	static void PipelineStateCSInitializeForUpdateVertex(ID3D12Device* device);

	/// <summary>
	/// パイプラインステートの初期化 共通関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="rootParameters">ルートパラメータ</param>
	/// <param name="numParameters">ルートパラメータ数</param>
	/// <param name="pipelineStateCSIndex">どのパイプラインか</param>
	/// <param name="filePath">HLSLのファイルパス</param>
	static void PipelineStateCSCommonInitialize(
		ID3D12Device* device,
		D3D12_ROOT_PARAMETER* rootParameters,
		uint32_t numParameters,
		PipelineStateCSIndex pipelineStateCSIndex,
		const std::wstring& filePath);

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClothGPU();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="scale">大きさ</param>
	/// <param name="div">分割数</param>
	/// <param name="textureName">テクスチャの名前(パスも含めて)</param>
	void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList, 
		const Vector2& scale, 
		const Vector2& div,
		const std::string textureName);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Update(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(ID3D12GraphicsCommandList* commandList, BaseCamera* camera);

	/// <summary>
	/// ImGui
	/// </summary>
	/// <param name="name">名前</param>
	void ImGuiDraw(const std::string& name);

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
	/// <param name="textureName">テクスチャの名前(パスも含めて)</param>
	void MaterialInitialize(const std::string textureName);

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
	///	バネバッファの初期化
	/// </summary>
	/// <param name="device"></param>
	void SpringBufferInitialize(ID3D12Device* device);

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
	/// CS初期化
	/// </summary>
	/// <param name="commandList"></param>
	void InitializeCS(ID3D12GraphicsCommandList* commandList);

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
	/// 面情報バッファ
	/// </summary>
	/// <param name="commandList"></param>
	void InitSurfaceCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// CS更新
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新 外部操作フェーズ
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateExternalOperationCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新 積分フェーズ
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateIntegralCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新 バネフェーズ
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateSpringCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新 衝突フェーズ
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateCollisionCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新 面フェーズ
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateSurfaceCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新 頂点フェーズ
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateVertexCS(ID3D12GraphicsCommandList* commandList);

private: // バリア

	/// <summary>
	/// UAVバリア 頂点
	/// </summary>
	/// <param name="commandList"></param>
	void UAVBarrierVertex(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UAVバリア 質点
	/// </summary>
	/// <param name="commandList"></param>
	void UAVBarrierMassPoint(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// リソースバリア
	/// </summary>
	/// <param name="commandList"></param>
	void ResouseBarrierToNonPixelShader(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// リソースバリア
	/// </summary>
	/// <param name="commandList"></param>
	void ResouseBarrierToUnorderedAccess(ID3D12GraphicsCommandList* commandList);

public: // 外部操作関数

	/// <summary>
	/// 重み設定
	/// </summary>
	/// <param name="y">y</param>
	/// <param name="x">x</param>
	/// <param name="isWight">重みをつけるか</param>
	void SetWeight(uint32_t y, uint32_t x, bool isWight);

	/// <summary>
	/// 位置設定
	/// </summary>
	/// <param name="y">y</param>
	/// <param name="x">x</param>
	/// <param name="position">位置</param>
	void SetPosition(uint32_t y, uint32_t x, const Vector3& position);

private: // その他関数

	/// <summary>
	/// バネ作成
	/// </summary>
	/// <param name="x">始点X</param>
	/// <param name="y">始点Y</param>
	/// <param name="offsetX">始点から終点への距離X</param>
	/// <param name="offsetY">始点から終点への距離Y</param>
	/// <param name="type">タイプ</param>
	void SpringGeneration(
		uint32_t x,
		uint32_t y,
		int32_t offsetX,
		int32_t offsetY, 
		TypeOfSpring type);

private: // その他関数

	/// <summary>
	/// マテリアル更新
	/// </summary>
	/// <param name="uvTransform">UVトランスフォーム</param>
	/// <param name="color">色</param>
	/// <param name="enableLighting">照明を有効にする</param>
	/// <param name="shininess">輝き</param>
	/// <param name="environmentCoefficient">環境係数</param>
	void MaterialUpdate(
		const EulerTransform& uvTransform, 
		const Vector4& color, 
		int enableLighting, 
		float shininess, 
		float environmentCoefficient);

public: // その他関数

	/// <summary>
	/// 衝突判定登録
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="collisionType">衝突判定のタイプ</param>
	void CollisionDataRegistration(
		const std::string& name,
		ClothGPUCollision::CollisionTypeIndex collisionType);

	/// <summary>
	/// 衝突判定削除
	/// </summary>
	/// <param name="name">名前</param>
	void CollisionDataDelete(const std::string& name);

	/// <summary>
	/// 衝突判定更新
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="collisionDataMap">衝突判定データ</param>
	void CollisionDataUpdate(
		const std::string& name,
		ClothGPUCollision::CollisionDataMap& collisionDataMap);

public: // アクセッサ

	// 布計算

	// 質量
	void SetMass(float mass) { clothCalcDataMap_->mass_ = mass; }
	float GetMass() { return clothCalcDataMap_->mass_; }

	// 重力
	void SetGravity(const Vector3& gravity) { clothCalcDataMap_->gravity_ = gravity; }
	Vector3 GetGravity() { return clothCalcDataMap_->gravity_; }

	// 風力
	void SetWind(const Vector3& wind) { clothCalcDataMap_->wind_ = wind; }
	Vector3 GetWind() { return clothCalcDataMap_->wind_; }
	
	// バネ強度
	void SetStiffness(float stiffness) { clothCalcDataMap_->stiffness_ = stiffness; }
	float GetStiffness() { return clothCalcDataMap_->stiffness_; }

	// 速度抵抗
	void SetSpeedResistance(float speedResistance) { clothCalcDataMap_->speedResistance_ = speedResistance; }
	float GetSpeedResistance() { return clothCalcDataMap_->speedResistance_; }
	
	// 構成バネ伸び抵抗
	void SetStructuralShrink(float structuralShrink) { clothCalcDataMap_->structuralShrink_ = structuralShrink; }
	float GetStructuralShrink() { return clothCalcDataMap_->structuralShrink_; }

	// 構成バネ縮み抵抗
	void SetStructuralStretch(float structuralStretch) { clothCalcDataMap_->structuralStretch_ = structuralStretch; }
	float GetStructuralStretch() { return clothCalcDataMap_->structuralStretch_; }

	// せん断バネ伸び抵抗
	void SetShearShrink(float shearShrink) { clothCalcDataMap_->shearShrink_ = shearShrink; }
	float GetShearShrink() { return clothCalcDataMap_->shearShrink_; }

	// せん断バネ縮み抵抗
	void SetShearStretch(float shearStretch) { clothCalcDataMap_->shearStretch_ = shearStretch; }
	float GetShearStretch() { return clothCalcDataMap_->shearStretch_; }

	// 曲げバネ伸び抵抗
	void SetBendingShrink(float bendingShrink) { clothCalcDataMap_->bendingShrink_ = bendingShrink; }
	float GetBendingShrink() { return clothCalcDataMap_->bendingShrink_; }

	// 曲げバネ縮み抵抗
	void SetBendingStretch(float bendingStretch) { clothCalcDataMap_->bendingStretch_ = bendingStretch; }
	float GetBendingStretch() { return clothCalcDataMap_->bendingStretch_; }

	// バネフェーズの反復回数
	void SetRelaxation(int32_t relaxation) { relaxation_ = relaxation; }
	int32_t GetRelaxation() { return relaxation_; }

	// マテリアル
	Material* GetMaterial() { return material_.get(); }

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
	ClothExternalOperationData* externalMap_ = nullptr;
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

private: // バネ(SRV)

	// バネバッファ
	std::array<ClothSpringBufferData, kClothSpringBufferStructIndexOfCount> clothSpringBufferStructs_;

	// バネの初期化時のindex情報
	std::array<uint32_t, kClothSpringBufferStructIndexOfCount> springInitNextIndexes_;

private: // CBV

	// 作成時データバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> createDataBuff_;
	// マップ
	ClothCreateData* createDataMap_ = nullptr;

	// WVPバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpBuff_;
	// WVPマップ
	ClothWVP* wvpMap_ = nullptr;

	// 布計算バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> clothCalcDataBuff_;
	// 布計算マップ
	ClothCalcData* clothCalcDataMap_ = nullptr;

	// 時間バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameBuff_;
	// 時間マップ
	PerFrame* perFrameMap_ = nullptr;

	// 数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> NumsBuff_;
	// 数マップ
	ClothNums* NumsMap_ = nullptr;

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

	// バネのindex
	Microsoft::WRL::ComPtr<ID3D12Resource> springIndexBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE springIndexUavHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE springIndexUavHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t springIndexUavIndexDescriptorHeap_ = 0;

private: // 変数

	// テクスチャハンドル
	uint32_t textureHandle_;

	// マテリアル
	std::unique_ptr<Material> material_;

	// バネフェーズの反復回数
	int32_t relaxation_;

	// 衝突データ
	std::list<std::pair<std::string, std::unique_ptr<ClothGPUCollision>>> collisionDatas_;

	// CSで更新をするか
	bool csUpdate_;

	// 初期化移動したか
	bool didYouMoveInit_;

};

