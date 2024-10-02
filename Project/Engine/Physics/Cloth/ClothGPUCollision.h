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
#include <variant>
#include "../../Math/PerFrame.h"

#pragma comment(lib, "dxcompiler.lib")

class ClothGPUCollision
{

public: // サブクラス

	/// <summary>
	/// 衝突タイプ
	/// </summary>
	enum CollisionTypeIndex {
		kCollisionTypeIndexPlane, // 平面
		kCollisionTypeIndexOfIndex // 数
	};

	/// <summary>
	/// 平面
	/// </summary>
	struct Plane
	{
		//法線
		Vector3 normal_;
		//距離
		float distance_;
	};

	// 衝突するデータマップ
	using CollisionDataMap = std::variant<Plane>;

public: // 静的メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	// CSの初期化

private: // バッファ作成関数

	/// <summary>
	/// 平面
	/// </summary>
	/// <param name="myData">自分</param>
	static void CreateBufferPlane(ClothGPUCollision* myData);

private: // CSの初期化関数

	/// <summary>
	/// 平面
	/// </summary>
	static void CSInitializePlane();

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device_;

	// バッファ作成関数群
	static std::array<std::function<void(ClothGPUCollision*)>, CollisionTypeIndex::kCollisionTypeIndexOfIndex> crateBufferFunctions_;

	// 衝突確認関数群
	//static std::array<std::function<void(ClothGPUCollision*)>, CollisionTypeIndex::kCollisionTypeIndexOfIndex> crateBufferFunctions_;

public: // 動的メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="collisionType">衝突タイプ</param>
	void Initialize(CollisionTypeIndex collisionType);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="collisionDataMap"></param>
	void Update(CollisionDataMap* collisionDataMap);

	/// <summary>
	/// CSの実行
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="massPointIndexSrvHandleGPU">質点のGPUハンドル</param>
	void ExecutionCS(ID3D12GraphicsCommandList* commandList, D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU);

private: // 動的メンバ変数

	// 衝突するデータバッファ CBV
	Microsoft::WRL::ComPtr<ID3D12Resource> collisionDataBuff_;
	// 衝突するデータマップ
	CollisionDataMap* collisionDataMap_ = nullptr;
	// 衝突タイプ
	CollisionTypeIndex collisionType_;

};

