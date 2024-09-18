#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include <vector>
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../3D/Material.h"
#include "../../Camera/BaseCamera.h"

class ClothModel
{

	/// <summary>
	/// 頂点
	/// </summary>
	struct VertexData {
		Vector4 position_; // 位置
	};

public:

	void Initialize(const Vector2& div);

	void Update(const std::vector<Vector3>& positions);
	
	void Draw(BaseCamera& camera);

private: 

	/// <summary>
	/// インデックスマッピング
	/// </summary>
	void IndexMapping();

public:

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファマップ
	ClothModel::VertexData* vertMap_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	//インデックスマップ
	uint32_t* indexMap_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	// 頂点の数
	uint32_t vertexNum_;

	// インデックスの数
	uint32_t indexNum_;

	// テクスチャハンドル
	uint32_t textureHandle_;

	// マテリアル
	std::unique_ptr<Material> matesial_;

	// 分割数
	Vector2 div_;

};

