#pragma once
#include "../BaseObstacle.h"
#include "../../../System/BossSystem.h"

class BossFog :
    public BaseObstacle
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// ボスシステム
	/// </summary>
	void SetBossSystem(BossSystem* bossSystem) { bossSystem_ = bossSystem; }

private: 

	// ボスシステム
	BossSystem* bossSystem_;

	// ボス戦中位置
	Vector3 battleTranslate_;

	//ボス戦以外位置 
	Vector3 translate_;

};

