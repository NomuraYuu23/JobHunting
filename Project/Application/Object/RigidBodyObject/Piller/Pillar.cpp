#include "Pillar.h"
#include "../../Character/Enemy/BaseEnemyAttack.h"
#include "../../Character/Player/PlayerAttack/PlayerAttack.h"

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

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.center_ += Matrix4x4::TransformNormal(colliderAddPos,worldTransform_.rotateMatrix_);
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void Pillar::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (broken_) {
		BaseRigidBodyObject::OnCollision(colliderPartner, collisionData);
	}
	else if(std::holds_alternative<BaseEnemyAttack*>(colliderPartner)){

		BaseEnemyAttack* attack = std::get<BaseEnemyAttack*>(colliderPartner);

		// 履歴確認
		if (attack->GetContactRecord().ConfirmHistory(serialNumber_)) {
			return;
		}

		// 履歴登録
		attack->GetContactRecord().AddHistory(serialNumber_);

		// 衝突処理
		Damage(1);

	}
	else if (std::holds_alternative<PlayerAttack*>(colliderPartner)) {

		PlayerAttack* attack = std::get<PlayerAttack*>(colliderPartner);

		// 履歴確認
		if (attack->GetContactRecord().ConfirmHistory(serialNumber_)) {
			return;
		}

		// 履歴登録
		attack->GetContactRecord().AddHistory(serialNumber_);

		// 衝突処理
		Damage(1);

	}

}

void Pillar::Damage(uint32_t damage)
{

	durable_ -= damage;

	if (durable_ <= 0) {
		broken_ = true;
		worldTransform_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransform_.parent_ = nullptr;
	}

}

void Pillar::SetParent(PillarFoundation* parent)
{

	parent_ = parent;

	worldTransform_.transform_.translate = {0.0f,0.0f,0.0f};
	worldTransform_.parent_ = parent_->GetWorldTransformAdress();

}
