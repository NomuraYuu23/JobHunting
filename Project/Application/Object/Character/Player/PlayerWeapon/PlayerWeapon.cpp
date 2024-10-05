#include "PlayerWeapon.h"

#include "../Player.h"
#include "../../../Engine/Physics/RigidBody/InertiaTensor.h"
#include "../../../Engine/Math/DeltaTime.h"

void PlayerWeapon::Initialize(LevelData::MeshData* data)
{

	BaseWeapon::Initialize(data);

	worldTransform_.transform_.scale = { 1.0f,1.0f,1.0f };
	worldTransform_.transform_.translate.y = 5.0f;
	worldTransform_.UpdateMatrix();

	rotate_ = { 0.0f, 0.0f, 3.14f };

}

void PlayerWeapon::Update()
{

	BaseWeapon::Update();

}

void PlayerWeapon::SetParent(Player* parent)
{

	parent_ = parent;

	LocalMatrixManager* localMatrixManager = parent_->GetLocalMatrixManager();

	std::vector<std::string> names = localMatrixManager->GetNodeNames();

	const std::string& parentName = "mixamorig:RightHand";

	for (uint32_t i = 0; i < names.size(); ++i) {
		if (parentName == names[i]) {
			parentNodeData_ = &localMatrixManager->GetNodeDatasAddress()->at(i);
			parentMatrix_ = &parent_->GetWorldTransformAdress()->parentMatrix_;
			break;
		}
	}

}

void PlayerWeapon::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	BaseWeapon::OnCollision(colliderPartner, collisionData);

}

void PlayerWeapon::WithParentsUpdate()
{

	// ノード追従
	NodeFollowing();

}

void PlayerWeapon::ParentlessUpdate()
{

	// 剛体の更新
	RigidBodyUpdate();

}

void PlayerWeapon::ColliderInitialize()
{

	BaseWeapon::ColliderInitialize();

}

void PlayerWeapon::ColliderUpdate()
{

	BaseWeapon::ColliderUpdate();

}
