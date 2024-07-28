#pragma once
#include <string>
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include "../../externals/DirectXTex/d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>

#include "../3D/WorldTransform.h"

class IObject
{

protected: 

	static ID3D12GraphicsCommandList* commandList_;

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	static void StaticInitialize(ID3D12GraphicsCommandList* commandList);

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

