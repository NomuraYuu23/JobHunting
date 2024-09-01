#include "FlagPole.h"

void FlagPole::Initialize(LevelData::MeshData* data)
{

	BaseObstacle::Initialize(data);

	// 旗の上部分
	upperPart_ = worldTransform_.GetWorldPosition();
	upperPart_.y = 10.0f;

	// 旗の下部分
	lowerPart_ = worldTransform_.GetWorldPosition();
	lowerPart_.y = 6.0f;

}
