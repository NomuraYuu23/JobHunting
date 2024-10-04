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

	// CSの初期化

	// CSの実行

private: // 静的メンバ変数


public: // 動的メンバ関数

	void Initialize(CollisionTypeIndex collisionType);

	void Update();

	void SetData(); // 衝突タイプでデータがあってるか確認する

private: // 動的メンバ変数

	// 衝突するデータバッファ CBV
	Microsoft::WRL::ComPtr<ID3D12Resource> collisionDataBuff_;
	// 衝突するデータマップ
	CollisionDataMap* collisionDataMap_ = nullptr;
	// 衝突タイプ
	CollisionTypeIndex collisionType;

};

