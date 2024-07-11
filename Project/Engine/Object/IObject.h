#pragma once
#include <string>

#include "../3D/WorldTransform.h"

class IObject
{

public: // 関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObject() = default;
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 名前取得
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return name_; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }
	bool GetIsDead() { return isDead_; }

protected: // 変数

	// 名前
	std::string name_;

	// トランスフォーム
	WorldTransform worldTransform_;

	// 死んでいるか
	bool isDead_ = false;

};

