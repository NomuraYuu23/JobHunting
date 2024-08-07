#pragma once
#include "../BaseRigidBodyObject.h"
class Pillar :
    public BaseRigidBodyObject
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

};

