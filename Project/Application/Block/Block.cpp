#include "Block.h"

void Block::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	material_->SetEnableLighting(BlinnPhongReflection);

	EulerTransform uvTransform;
	uvTransform.scale = { worldTransform_.transform_.scale.x / 2.0f, worldTransform_.transform_.scale.z / 2.0f,1.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.translate = { 0.0f,0.0f,0.0f };
	material_->SetUvTransform(uvTransform);

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
