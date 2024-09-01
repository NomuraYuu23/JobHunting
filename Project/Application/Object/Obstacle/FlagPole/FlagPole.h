#pragma once
#include "../BaseObstacle.h"
class FlagPole :
    public BaseObstacle
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	Vector3 GetUpperPart() { return upperPart_; }
	Vector3 GetLowerPart() { return lowerPart_; }

private:

	// 旗の上部分
	Vector3 upperPart_{};
	
	// 旗の下部分
	Vector3 lowerPart_{};

};

