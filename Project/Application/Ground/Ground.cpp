#include "Ground.h"

void Ground::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void Ground::Update()
{
}

void Ground::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void Ground::ImGuiDraw()
{
}

void Ground::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{




}
