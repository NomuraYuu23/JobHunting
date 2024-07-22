#include "Ground.h"

void Ground::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	material_->SetEnableLighting(BlinnPhongReflection);

	EulerTransform uvTransform;
	uvTransform.scale = { 100.0f, 100.0f,1.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.translate = { 0.0f,0.0f,0.0f };
	material_->SetUvTransform(uvTransform);

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
