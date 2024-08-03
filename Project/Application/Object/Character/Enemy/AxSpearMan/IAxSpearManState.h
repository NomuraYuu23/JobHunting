#pragma once
#include <cstdint>

class AxSpearMan;

// 斧槍男の状態名
enum AxSpearManState {
	kAxSpearManStateRoot, // 通常
	kAxSpearManStateDashSwingDown, // ダッシュ振り下ろし
	kAxSpearManStateWaitSee, // 様子見
	kAxSpearManStateShakeOff, // 振り払い
	kAxSpearManState3Consecutive, // 3連撃
	kAxSpearManStateBeam, // ビーム
	kAxSpearManStateOfCount // 数
};

class IAxSpearManState
{

protected:

	// 状態番号
	static uint32_t axSpearManStateNo_;

	// モーション番号
	static uint32_t axSpearManMotionNo_;

	// 自身
	static AxSpearMan* axSpearMan_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IAxSpearManState() = default;

	// 状態番号
	uint32_t GetAxSpearManStateNo() { return axSpearManStateNo_; }

	// モーション番号
	uint32_t GetAxSpearManMotionNo() { return axSpearManMotionNo_; }

	// 自身をセット
	void SetAxSpearMan(AxSpearMan* axSpearMan) { axSpearMan_ = axSpearMan; }

};

