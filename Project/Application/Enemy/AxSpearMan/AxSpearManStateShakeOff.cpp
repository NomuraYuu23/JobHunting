#include "AxSpearManStateShakeOff.h"
#include "AxSpearMan.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Player/Player.h"

const AxSpearManStateShakeOff::ConstAttack AxSpearManStateShakeOff::kConstAttak = {
		{ 10, 20, 10, 25}, { 0.0f, 0.0f, 0.1f, 0.0f}
};

void AxSpearManStateShakeOff::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	attack_ = axSpearMan_->GetAttack();

	// 媒介変数
	parameter_ = 0.0f;

	// フレーム数
	periodFrame_ = static_cast<uint32_t>(kConstAttak.time_[kPosture]);

	// フェーズ
	inPhase_ = kPosture;

	AttackInitialize();

	axSpearManStateNo_ = AxSpearManState::kAxSpearManStateShakeOff;

	attack_->ClearContactRecord();

}

void AxSpearManStateShakeOff::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	Attack();

	AttackPhaseFinished();

	axSpearMan_->SetReceiveCommand(false);

}

void AxSpearManStateShakeOff::AttackInitialize()
{

	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	periodFrame_ = kConstAttak.time_[inPhase_];

	//モーションネーム
	axSpearManMotionNo_ = kAxSpearManMotionShakeOff;

}

void AxSpearManStateShakeOff::Attack()
{

	Move();

	// コライダー更新
	if ((inPhase_ == static_cast<uint32_t>(ComboPhase::kAttack))) {
		attack_->Update();
		attack_->SetIsAttackJudgment(true);
	}
	else {
		DontAttack();
	}

}

void AxSpearManStateShakeOff::DontAttack()
{

	attack_->Stop();

}

void AxSpearManStateShakeOff::Move()
{
}

void AxSpearManStateShakeOff::AttackPhaseFinished()
{

	if (parameter_ >= 1.0f) {
		inPhase_++;
		AttackInitialize();
		if (inPhase_ == static_cast<uint32_t>(ComboPhase::kCountOfComboPhase)) {
			axSpearManStateNo_ = AxSpearManState::kAxSpearManStateRoot;
			DontAttack();
		}
		else {
			DontAttack();
		}
	}

}
