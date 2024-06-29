#include "Block.h"

void Block::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void Block::Update()
{
}

void Block::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void Block::ImGuiDraw()
{
}

void Block::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}
