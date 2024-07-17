#include "AxSpearManBeam.h"

void AxSpearManBeam::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	worldTransform_.transform_.scale = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	uvTransform.translate = { 0.0f,0.0f,0.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.scale = { 1.0f,1.0f,1.0f };

}

void AxSpearManBeam::Update(const Vector3& position, const Matrix4x4& rotateMatrix, const Vector3& size)
{

	uvTransform.translate.y = std::fmodf(uvTransform.translate.y + kUvTransformSpeed, 1.0f);

	material_->SetUvTransform(uvTransform);

	position_ = position;

	rotateMatrix_ = rotateMatrix;

	size_ = size;

	worldTransform_.transform_.translate = position_;
	worldTransform_.transform_.scale = size_;
	worldTransform_.UpdateMatrix(rotateMatrix_);

}
