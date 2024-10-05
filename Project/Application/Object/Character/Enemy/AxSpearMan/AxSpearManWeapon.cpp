#include "AxSpearManWeapon.h"
#include "AxSpearMan.h"
#include "../../../Engine/Physics/RigidBody/InertiaTensor.h"
#include "../../../Engine/Math/DeltaTime.h"

void AxSpearManWeapon::Initialize(LevelData::MeshData* data)
{

	BaseWeapon::Initialize(data);

	worldTransform_.transform_.scale = { 1.0f,1.0f,1.0f };
	worldTransform_.transform_.translate.y = 5.0f;
	worldTransform_.UpdateMatrix();

	rotate_ = { 0.0f, -1.0f, 0.0f };

}

void AxSpearManWeapon::Update()
{

	BaseWeapon::Update();

}

void AxSpearManWeapon::SetParent(AxSpearMan* parent)
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

void AxSpearManWeapon::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{


	BaseWeapon::OnCollision(colliderPartner, collisionData);

}

void AxSpearManWeapon::WithParentsUpdate()
{

	// ノード追従
	NodeFollowing();

}

void AxSpearManWeapon::ParentlessUpdate()
{

	// 剛体の更新
	RigidBodyUpdate();

}

void AxSpearManWeapon::ColliderInitialize()
{

	BaseWeapon::ColliderInitialize();

}

void AxSpearManWeapon::ColliderUpdate()
{

	BaseWeapon::ColliderUpdate();

}
