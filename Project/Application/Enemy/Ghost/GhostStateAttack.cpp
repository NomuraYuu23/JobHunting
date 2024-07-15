#include "GhostStateAttack.h"
#include "Ghost.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Input/Input.h"
#include "../../Player/Player.h"

// コンボ定数表
const std::array<GhostStateAttack::ConstAttack, GhostStateAttack::kComboNum> GhostStateAttack::kConstAttacks = {
	{
		{ { 25, 25, 25, 125}, { 0.1f, 0.0f, 0.2f, 0.0f} },
	}
};

void GhostStateAttack::Initialize()
{

	attack_ = ghost_->GetAttack();

	// 媒介変数
	parameter_ = 0.0f;

	// フレーム数
	periodFrame_ = 10;

	// コンボインデックス
	comboIndex_ = 0;

	// コンボフェーズ
	inComboPhase_ = 0;

	// コンボネクスト
	comboNext_ = false;

	AttackInitialize();

	ghostStateNo_ = GhostState::kGhostStateAttack;

}

void GhostStateAttack::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	AttackCombo1st();

	if (parameter_ > step) {
		AttackComboContinuationJudgment();
	}
	AttackComboPhaseFinished();

	ghost_->SetReceiveCommand(false);

}

void GhostStateAttack::AttackInitialize()
{


	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	periodFrame_ = kConstAttacks[comboIndex_].time_[inComboPhase_];

	//モーションネーム
	ghostMotionNo_ = kGhostMotionAttack;

	attack_->SetCenter(attackCenter_);
	attack_->SetRadius(attackRadius_);

}

void GhostStateAttack::AttackComboContinuationJudgment()
{

	Input* input = Input::GetInstance();

	// ここに追加でスタミナ処理
	if (input->TriggerJoystick(kJoystickButtonRB)) {
		comboNext_ = true;
	}

}

void GhostStateAttack::AttackComboPhaseFinished()
{

	if (parameter_ >= 1.0f) {
		inComboPhase_++;
		AttackInitialize();
		if (inComboPhase_ == static_cast<uint32_t>(ComboPhase::kCountOfComboPhase)) {
			AttackComboFinished();
			DontAttack();
		}
		else if (inComboPhase_ != static_cast<uint32_t>(ComboPhase::kSwing)) {
			DontAttack();
		}
	}

}

void GhostStateAttack::AttackComboFinished()
{


	inComboPhase_ = 0;
	// コンボ継続か
	if (comboNext_) {
		//フラグリセット
		comboNext_ = false;
		if (comboIndex_ < kComboNum - 1) {
			comboIndex_++;
		}
		else {
			comboIndex_ = 0;
		}
		AttackInitialize();
		attack_->ClearContactRecord();
	}
	else {
		ghostStateNo_ = GhostState::kGhostStateRoot;
		attack_->ClearContactRecord();
	}


}

void GhostStateAttack::AttackCombo1st()
{

	Move();

	// コライダー更新
	if (inComboPhase_ == static_cast<uint32_t>(ComboPhase::kSwing) && parameter_ >= 0.5f) {
		attack_->Update();
		attack_->SetIsAttackJudgment(true);
	}

}

void GhostStateAttack::DontAttack()
{

	attack_->Stop();

}

void GhostStateAttack::Move()
{

	WorldTransform* worldTransform = ghost_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	Player* player = ghost_->GetPlayer();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	// 移動量
	Vector3 move = player->GetWorldTransformAdress()->GetWorldPosition() - worldTransform->GetWorldPosition();

	// 移動量に速さを反映
	move = Vector3::Multiply(kConstAttacks[comboIndex_].speed_[inComboPhase_], Vector3::Normalize(move));

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	Vector3 targetDirection = {0.0f,0.0f,0.0f};
	targetDirection.x = Vector3::Normalize(move).x;
	targetDirection.z = Vector3::Normalize(move).z;
	float targetAngleT = 0.02f;

	// 角度補間
	worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection, targetAngleT);

	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

}
