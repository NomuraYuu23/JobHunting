#include "BasePhysicsObject.h"

void BasePhysicsObject::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	material_->SetEnableLighting(BlinnPhongReflection);

}

void BasePhysicsObject::Update()
{
}

void BasePhysicsObject::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void BasePhysicsObject::ImGuiDraw()
{
}

void BasePhysicsObject::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}
