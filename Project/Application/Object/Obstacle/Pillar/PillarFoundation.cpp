#include "PillarFoundation.h"

void PillarFoundation::Initialize(LevelData::MeshData* data)
{

	BaseObstacle::Initialize(data);

	EulerTransform uvTransform;
	uvTransform.scale = { worldTransform_.transform_.scale.x / 2.0f, worldTransform_.transform_.scale.z / 2.0f,1.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.translate = { 0.0f,0.0f,0.0f };
	material_->SetUvTransform(uvTransform);

}
