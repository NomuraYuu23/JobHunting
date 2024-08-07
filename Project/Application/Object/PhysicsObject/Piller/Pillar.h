#pragma once
#include "../BasePhysicsObject.h"
class Pillar :
    public BasePhysicsObject
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

};

