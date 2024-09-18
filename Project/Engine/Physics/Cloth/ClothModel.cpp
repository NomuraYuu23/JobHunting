#include "ClothModel.h"
#include "../../base/DirectXCommon.h"
#include "../../base/BufferResource.h"

void ClothModel::Initialize(const Vector2& div)
{

	// 分割数
	div_ = div;

	// 頂点の数
	vertexNum_ = (static_cast<uint32_t>(div_.x) + 1) * static_cast<uint32_t>(div_.y) + 1;

	// インデックスの数
	indexNum_ = static_cast<uint32_t>(div_.x) * static_cast<uint32_t>(div_.y) * 6;


	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// 頂点バッファ
	vertBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(ClothModel::VertexData) + 0xff) & ~0xff) * vertexNum_);
	//リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	vbView_.SizeInBytes = UINT(sizeof(ClothModel::VertexData) * vertexNum_);
	//1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(ClothModel::VertexData);
	//書き込むためのアドレスを取得
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertMap_));
	// 一度0でマッピング
	for (uint32_t i = 0; i < vertexNum_; ++i) {
		vertMap_[i].position_ = { 0.0f,0.0f,0.0f,1.0f };
	}

	//インデックスリソースを作る
	indexBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(uint32_t) + 0xff) & ~0xff) * indexNum_);
	//リソースの先頭のアドレスから使う
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	ibView_.SizeInBytes = sizeof(uint32_t) * indexNum_;
	//インデックスはuint32_tとする
	ibView_.Format = DXGI_FORMAT_R32_UINT;
	//インデックスリソースにデータを書き込む
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap_));

	// インデックスマッピング
	IndexMapping();

	// テクスチャハンドル
	textureHandle_ = 0;

	// マテリアル
	matesial_.reset();

}

void ClothModel::Update(const std::vector<Vector3>& positions)
{

	for (uint32_t i = 0; i < vertexNum_; ++i) {
		vertMap_[i].position_.x = positions[i].x;
		vertMap_[i].position_.y = positions[i].y;
		vertMap_[i].position_.z = positions[i].z;
		vertMap_[i].position_.w = 1.0f;
	}

}

void ClothModel::IndexMapping()
{

	uint32_t index = 0;

	for(uint32_t y = 0; y < static_cast<uint32_t>(div_.y); ++y){
		for (uint32_t x = 0; x < static_cast<uint32_t>(div_.x); ++x) {

			// 一つ目の三角形
			// 左上
			indexMap_[index] = y * static_cast<uint32_t>(div_.x) + x;
			index++;
			// 右上
			indexMap_[index] = y * static_cast<uint32_t>(div_.x) + x + 1;
			index++;
			// 左下
			indexMap_[index] = (y + 1) * static_cast<uint32_t>(div_.x) + x;
			index++;

			// 二つ目の三角形
			// 右上
			indexMap_[index] = y * static_cast<uint32_t>(div_.x) + x + 1;
			index++;
			// 左下
			indexMap_[index] = (y + 1) * static_cast<uint32_t>(div_.x) + x;
			index++;
			// 右下
			indexMap_[index] = (y + 1) * static_cast<uint32_t>(div_.x) + x + 1;
			index++;
		
		}
	}


}
