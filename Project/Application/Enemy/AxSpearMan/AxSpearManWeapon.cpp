#include "AxSpearManWeapon.h"
#include "AxSpearMan.h"
#include "../../../Engine/Physics/InertiaTensor.h"
#include "../../../Engine/Math/DeltaTime.h"

void AxSpearManWeapon::Initialize(LevelData::MeshData* data)
{

	BaseWeapon::Initialize(data);

	worldTransform_.transform_.scale = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	parentName_ = "AxSpearMan00";

	rotate_ = { 0.0f, -1.57f, 0.0f };

	// 剛体の初期化
	RigidBodyInitialize();

	// コライダーの初期化
	ColliderInitialize();

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

	// 位置行列
	Vector3 pos = {
	parentNodeData_->matrix.m[3][0],
	parentNodeData_->matrix.m[3][1],
	parentNodeData_->matrix.m[3][2] };

	pos = Matrix4x4::Transform(pos, *parentMatrix_);

	// 回転行列
	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(parentNodeData_->offsetMatrix, parentNodeData_->matrix);
	rotateMatrix.m[3][0] = 0.0f;
	rotateMatrix.m[3][1] = 0.0f;
	rotateMatrix.m[3][2] = 0.0f;

	Matrix4x4 parentRotateMatrix = *parentMatrix_;

	parentRotateMatrix.m[3][0] = 0.0f;
	parentRotateMatrix.m[3][1] = 0.0f;
	parentRotateMatrix.m[3][2] = 0.0f;

	rotateMatrix = Matrix4x4::Multiply(Matrix4x4::Multiply(rotateMatrix, parentRotateMatrix), Matrix4x4::MakeRotateXYZMatrix(rotate_));

	worldTransform_.worldMatrix_ = Matrix4x4::Multiply(rotateMatrix, Matrix4x4::MakeTranslateMatrix(pos));

}

void AxSpearManWeapon::ParentlessUpdate()
{

	// 剛体の更新
	RigidBodyUpdate();

}

void AxSpearManWeapon::ColliderInitialize()
{

	// OBB
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void AxSpearManWeapon::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();

	Vector3 ColliderMove = { 0.0f,2.5f, 0.0f };

	ColliderMove = Matrix4x4::TransformNormal(ColliderMove, worldTransform_.rotateMatrix_);

	obb.center_ += ColliderMove;
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}
