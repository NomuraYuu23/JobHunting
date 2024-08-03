#include "BaseObstacle.h"

void BaseObstacle::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	material_->SetEnableLighting(BlinnPhongReflection);

}

void BaseObstacle::Update()
{
}

void BaseObstacle::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void BaseObstacle::ImGuiDraw()
{
}

void BaseObstacle::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}
