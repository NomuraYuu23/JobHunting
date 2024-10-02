#include "ClothGPUCollision.h"
#include "../../base/BufferResource.h"

// デバイス
ID3D12Device* ClothGPUCollision::device_;
// バッファ作成関数群
std::array<std::function<void(ClothGPUCollision*)>, ClothGPUCollision::CollisionTypeIndex::kCollisionTypeIndexOfIndex> ClothGPUCollision::crateBufferFunctions_;

void ClothGPUCollision::Initialize()
{

	// 関数初期化
	crateBufferFunctions_[kCollisionTypeIndexPlane] = ClothGPUCollision::CreateBufferPlane;

}

void ClothGPUCollision::CreateBufferPlane(ClothGPUCollision* myData)
{

	// 衝突するデータバッファ CBV
	myData->collisionDataBuff_ = BufferResource::CreateBufferResource(device_, (sizeof(ClothGPUCollision::Plane) + 0xff) & ~0xff);
	myData->collisionDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&myData->collisionDataMap_));
	// 衝突するデータマップ
	ClothGPUCollision::Plane* map = std::get_if<ClothGPUCollision::Plane>(myData->collisionDataMap_);
	map->distance_ = 1.0f;
	map->normal_ = { 0.0f,0.0f, 1.0f };

}

void ClothGPUCollision::CSInitializePlane()
{
}

void ClothGPUCollision::Initialize(CollisionTypeIndex collisionType)
{

	// 衝突タイプ
	collisionType_ = collisionType;

	// バッファ作成
	crateBufferFunctions_[collisionType_](this);

}
