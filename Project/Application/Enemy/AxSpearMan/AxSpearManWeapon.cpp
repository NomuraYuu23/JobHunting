#include "AxSpearManWeapon.h"

void AxSpearManWeapon::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	worldTransform_.transform_.scale = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

}

void AxSpearManWeapon::Update()
{

	Vector3 pos = {
	parentNodeData_->matrix.m[3][0],
	parentNodeData_->matrix.m[3][1],
	parentNodeData_->matrix.m[3][2] };

	pos = Matrix4x4::Transform(pos, *parentMatrix_);

	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(parentNodeData_->offsetMatrix, parentNodeData_->matrix);
	rotateMatrix.m[3][0] = 0.0f;
	rotateMatrix.m[3][1] = 0.0f;
	rotateMatrix.m[3][2] = 0.0f;

	worldTransform_.worldMatrix_ = Matrix4x4::Multiply(rotateMatrix, Matrix4x4::MakeTranslateMatrix(pos));

}

void AxSpearManWeapon::SetParent(NodeData* parentNodeData, Matrix4x4* parentMatrix)
{

	parentNodeData_ = parentNodeData;

	parentMatrix_ = parentMatrix;

}
