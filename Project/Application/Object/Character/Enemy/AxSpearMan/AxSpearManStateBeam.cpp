#include "AxSpearManStateBeam.h"
#include "AxSpearMan.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Player/Player.h"

const AxSpearManStateBeam::ConstAttack AxSpearManStateBeam::kConstAttak = {
	{ 20, 40, 60, 30 },
	{ -0.001f, -0.001f, -0.1f, 0.0f}
};

void AxSpearManStateBeam::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	attack_ = axSpearMan_->GetBeamAttack();

	// 媒介変数
	parameter_ = 0.0f;

	// フレーム数
	periodFrame_ = static_cast<uint32_t>(kConstAttak.time_[kPosture]);

	// フェーズ
	inPhase_ = kPosture;

	AttackInitialize();

	axSpearManStateNo_ = AxSpearManState::kAxSpearManStateBeam;

	attack_->ClearContactRecord();

}

void AxSpearManStateBeam::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	Attack();

	AttackPhaseFinished();

	axSpearMan_->SetReceiveCommand(false);

}

void AxSpearManStateBeam::AttackInitialize()
{

	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	periodFrame_ = kConstAttak.time_[inPhase_];

	//モーションネーム
	axSpearManMotionNo_ = kAxSpearManMotionBeam;

	attack_->SetCenter(attackCenter_);
	attack_->SetRotate(axSpearMan_->GetWorldTransformAdress()->rotateMatrix_);
	attack_->SetSize(attackSize_);

}

void AxSpearManStateBeam::Attack()
{

	if (inPhase_ <= static_cast<uint32_t>(ComboPhase::kCharge)) {
		Aim();
	}

	if (inPhase_ <= static_cast<uint32_t>(ComboPhase::kAttack)) {
		Recoil();
	}

	// コライダー更新
	bool isAttack = (inPhase_ == static_cast<uint32_t>(ComboPhase::kAttack));

	if (isAttack && parameter_ >= 0.0f) {

		// 位置と回転、大きさを変更
		attackCenter_.z += attackCenterSpeed_;
		attackCenter_.y = height_;
		attackSize_.z += attackCenterSpeed_;
		attack_->SetCenter(attackCenter_);
		attack_->SetRotate(axSpearMan_->GetWorldTransformAdress()->rotateMatrix_);
		attack_->SetSize(attackSize_);

		attack_->Update();
		attack_->SetIsAttackJudgment(true);
		axSpearMan_->SetIsBeamDraw(true);
	}
	else {
		axSpearMan_->SetIsBeamDraw(false);
		DontAttack();
	}

}

void AxSpearManStateBeam::DontAttack()
{

	attack_->Stop();

}

void AxSpearManStateBeam::AttackPhaseFinished()
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

void AxSpearManStateBeam::Recoil()
{

	WorldTransform* worldTransform = axSpearMan_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	Player* player = axSpearMan_->GetPlayer();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	WorldTransform* playerWorldTransform = player->GetWorldTransformAdress();

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };
	// 移動量に速さを反映
	move = Vector3::Multiply(kConstAttak.speed_[inPhase_], Vector3::Normalize(move));

	// 移動ベクトルをカメラの角度だけ回転する
	move = Matrix4x4::TransformNormal(move, worldTransform->rotateMatrix_);

	// 移動(ビームを放っているとき)
	if (inPhase_ == static_cast<uint32_t>(ComboPhase::kAttack)) {

		velocity.x = move.x;
		velocity.z = move.z;

		worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

	}

}

void AxSpearManStateBeam::Aim()
{

	WorldTransform* worldTransform = axSpearMan_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	Player* player = axSpearMan_->GetPlayer();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	WorldTransform* playerWorldTransform = player->GetWorldTransformAdress();

	// 向き
	Vector3 direction = Vector3::Normalize(Vector3::Subtract(playerWorldTransform->GetWorldPosition(), worldTransform->GetWorldPosition()));

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };

	Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, direction);

	// 移動ベクトルをカメラの角度だけ回転する
	move = Matrix4x4::TransformNormal(move, rotateMatrix);

	// 移動方向に見た目を合わせる(Y軸)
	if (inPhase_ <= static_cast<uint32_t>(ComboPhase::kCharge)) {

		targetDirection_.x = Vector3::Normalize(move).x;
		targetDirection_.z = Vector3::Normalize(move).z;

		// 角度補間
		worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);

	}

}
