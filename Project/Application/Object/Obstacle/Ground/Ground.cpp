#include "Ground.h"

void Ground::Initialize(LevelData::MeshData* data)
{

	BaseObstacle::Initialize(data);

	EulerTransform uvTransform;
	uvTransform.scale = { 100.0f, 100.0f,1.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.translate = { 0.0f,0.0f,0.0f };
	material_->SetUvTransform(uvTransform);

}
