#include "AxSpearManStateDashSwingDown.h"
#include "AxSpearMan.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Player/Player.h"

const AxSpearManStateDashSwingDown::ConstAttack AxSpearManStateDashSwingDown::kConstAttak = {
		{ 10, 20, 40, 10, 25}, { 0.0f, 0.3f, 0.0f, 0.1f, 0.0f}
};

void AxSpearManStateDashSwingDown::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	attack_ = axSpearMan_->GetAttack();

	// 媒介変数
	parameter_ = 0.0f;

	// フレーム数
	periodFrame_ = 10;

	// フェーズ
	inPhase_ = 0;

	AttackInitialize();

	axSpearManStateNo_ = AxSpearManState::kAxSpearManStateDashSwingDown;

	attack_->ClearContactRecord();

}

void AxSpearManStateDashSwingDown::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	Attack();

	AttackPhaseFinished();

	axSpearMan_->SetReceiveCommand(false);

}

void AxSpearManStateDashSwingDown::AttackInitialize()
{

	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	periodFrame_ = kConstAttak.time_[inPhase_];

	//モーションネーム
	axSpearManMotionNo_ = kAxSpearManMotionDashSwingDown;

	attack_->SetCenter(attackCenter_);
	attack_->SetRadius(attackRadius_);

}

void AxSpearManStateDashSwingDown::Attack()
{

	Move();

	// コライダー更新
	if ((inPhase_ == static_cast<uint32_t>(ComboPhase::kSwing) && parameter_ >= 0.3f) ||
		(inPhase_ == static_cast<uint32_t>(ComboPhase::kRecovery) && parameter_ <= 0.2f)) {
		attack_->Update();
		attack_->SetIsAttackJudgment(true);
	}
	else {
		DontAttack();
	}

}

void AxSpearManStateDashSwingDown::DontAttack()
{

	attack_->Stop();

}

void AxSpearManStateDashSwingDown::Move()
{

	WorldTransform* worldTransform = axSpearMan_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	Player* player = axSpearMan_->GetPlayer();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	WorldTransform* playerWorldTransform = player->GetWorldTransformAdress();

	Vector3 direction = worldTransform->direction_;
	// 向き
	if (inPhase_ <= kCharge) {
		direction = Vector3::Normalize(Vector3::Subtract(playerWorldTransform->GetWorldPosition(), worldTransform->GetWorldPosition()));
		Vector3 directionCheck = {
			std::fabsf(std::fabsf(direction.x) - std::fabsf(worldTransform->direction_.x)),
			std::fabsf(std::fabsf(direction.y) - std::fabsf(worldTransform->direction_.y)),
			std::fabsf(std::fabsf(direction.z) - std::fabsf(worldTransform->direction_.z)) };
		if (directionCheck.x > directionLimit.x) {
			if (direction.x < 0.0f) {
				direction.x = -directionLimit.x;
			}
			else {
				direction.x = directionLimit.x;
			}
		}
		if (directionCheck.y > directionLimit.y) {
			if (direction.y < 0.0f) {
				direction.y = -directionLimit.y;
			}
			else {
				direction.y = directionLimit.y;
			}
		}
		if (directionCheck.z > directionLimit.z) {
			if (direction.z < 0.0f) {
				direction.z = -directionLimit.z;
			}
			else {
				direction.z = directionLimit.z;
			}
		}
	}

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };

	// 移動量に速さを反映
	move = Vector3::Multiply(kConstAttak.speed_[inPhase_], Vector3::Normalize(move));

	Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, direction);

	// 移動ベクトルをカメラの角度だけ回転する
	move = Matrix4x4::TransformNormal(move, rotateMatrix);

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

void AxSpearManStateDashSwingDown::AttackPhaseFinished()
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
