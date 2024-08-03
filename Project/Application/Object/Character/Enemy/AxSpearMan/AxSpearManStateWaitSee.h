#pragma once
#include "IAxSpearManState.h"
#include "../../../Engine/Math/DeltaTime.h"
class AxSpearManStateWaitSee :
    public IAxSpearManState
{

public:
	
	// フェーズ
	enum Phase {
		kPhaseStart, // 開始
		kPhaseMove, // 移動 
		KPhaseEnd, // 終わり
		kPhaseOfCount,
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	void StartUpdate();
	void MoveUpdate();
	void EndUpdate();

	/// <summary>
	/// 走る
	/// </summary>
	void Move();

private :
	
	// 移動アニメーションのタイム
	const double kAnimationTimeWhenMoving = 15.0 * kDeltaTime_;

	// 終了時のタイム
	const double kEndTime = 30.0 * kDeltaTime_;

	// 終了する距離
	const float kEndDistance = 5.0f;

	//フェーズ
	Phase inPhase_;

	// タイマー
	double timer_;

	// 距離
	float distance_;

};

