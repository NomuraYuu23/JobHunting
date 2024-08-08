#include "Pillar.h"

void Pillar::Initialize(LevelData::MeshData* data)
{

	BaseRigidBodyObject::Initialize(data);
	// 壊れたか
	broken_ = false;

	// 耐久値
	durable_ = kInitDurable_;

}

void Pillar::Update()
{

	if (broken_) {
		BaseRigidBodyObject::Update();
	}
	else {
		worldTransform_.UpdateMatrix();
	}

}

void Pillar::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (broken_) {
		BaseRigidBodyObject::OnCollision(colliderPartner, collisionData);
	}
	else if(std::holds_alternative<BaseEnemyAttack*>(colliderPartner) || std::holds_alternative<PlayerAttack*>(colliderPartner)){

	}

}
