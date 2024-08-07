#pragma once
#include "../BaseObstacle.h"
class Pillar :
    public BaseObstacle
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

};

