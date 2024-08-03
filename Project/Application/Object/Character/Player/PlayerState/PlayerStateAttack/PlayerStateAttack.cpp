#include "PlayerStateAttack.h"
#include "../../Player.h"
#include "../../../../../../Engine/Math/Ease.h"

// コンボ定数表
const std::array<PlayerStateAttack::ConstAttack, PlayerStateAttack::kComboNum> PlayerStateAttack::kConstAttaks = {
	{
		{ { 16, 16, 10, 10}, { 0.1f, 0.0f, 0.2f, 0.0f} },
		{ { 11, 11, 9, 9}, { 0.1f, 0.0f, 0.2f, 0.0f} },
	}
};

void PlayerStateAttack::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	attack_ = player_->GetPlayerAttack();

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

	playerStateNo_ = PlayerState::kPlayerStateAttack;

}

void PlayerStateAttack::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	if (comboIndex_ == 0) {
		AttackCombo1st();
	}
	else {
		AttackCombo2nd();
	}

	if (parameter_ > step) {
		AttackComboContinuationJudgment();
	}
	AttackComboPhaseFinished();

	player_->SetReceiveCommand(false);

}

void PlayerStateAttack::AttackInitialize()
{

	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	periodFrame_ = kConstAttaks[comboIndex_].time_[inComboPhase_];

	//モーションネーム
	if (comboIndex_ == 0) {
		playerMotionNo_ = kPlayerMotionAttack1st;
	}
	else {
		playerMotionNo_ = kPlayerMotionAttack2nd;
	}

}

void PlayerStateAttack::AttackComboContinuationJudgment()
{

	Input* input = Input::GetInstance();

	// ここに追加でスタミナ処理
	if (input->TriggerJoystick(kJoystickButtonRB)) {
		comboNext_ = true;
	}

}

void PlayerStateAttack::AttackComboPhaseFinished()
{

	if (parameter_ >= 1.0f) {
		inComboPhase_++;
		AttackInitialize();
		if (inComboPhase_ == static_cast<uint32_t>(ComboPhase::kCountOfComboPhase)) {
			AttackComboFinished();
			DontAttack();
		}
		else if (inComboPhase_ >= static_cast<uint32_t>(ComboPhase::kSwing)) {
			attack_->SetIsAttackJudgment(true);
		}
		else {
			DontAttack();
		}
	}

}

void PlayerStateAttack::AttackComboFinished()
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
		playerStateNo_ = PlayerState::kPlayerStateRoot;
		attack_->ClearContactRecord();
	}

}

void PlayerStateAttack::AttackCombo1st()
{

	Move();

	// コライダー更新
	if (inComboPhase_ == static_cast<uint32_t>(ComboPhase::kSwing)) {
		attack_->Update();
	}

}

void PlayerStateAttack::AttackCombo2nd()
{

	Move();

	// コライダー更新
	if (inComboPhase_ == static_cast<uint32_t>(ComboPhase::kSwing)) {
		attack_->Update();
	}

}

void PlayerStateAttack::DontAttack()
{

	attack_->Stop();

}

void PlayerStateAttack::Move()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	BaseCamera* camera = player_->GetCamera();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	// 移動量
	Vector3 move = worldTransform->direction_;

	// 移動量に速さを反映
	move = Vector3::Multiply(kConstAttaks[comboIndex_].speed_[inComboPhase_], Vector3::Normalize(move));

	// カメラの角度から回転行列を計算する
	//Matrix4x4 rotateMatrix = m4Calc_->MakeRotateXYZMatrix(camera->GetTransform().rotate);

	// 移動ベクトルをカメラの角度だけ回転する
	//move = m4Calc_->TransformNormal(move, rotateMatrix);

	//　親がいれば
	//if (worldTransform->parent_) {
		//rotateMatrix = m4Calc_->Inverse(worldTransform->parent_->rotateMatrix_);
		//move = m4Calc_->TransformNormal(move, rotateMatrix);
	//}

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	// あたん
	targetDirection_.x = Vector3::Normalize(move).x;
	targetDirection_.z = Vector3::Normalize(move).z;

	// 角度補間
	worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);

	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

}
