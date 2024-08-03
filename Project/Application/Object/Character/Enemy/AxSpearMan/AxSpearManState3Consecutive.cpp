#include "AxSpearManState3Consecutive.h"
#include "AxSpearMan.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Player/Player.h"

const AxSpearManState3Consecutive::ConstAttack AxSpearManState3Consecutive::kConstAttak = {
	{ 40, 10, 10, 20, 10, 10, 40, 10, 50}, 
	{ -0.001f, 0.1f, 0.0f, -0.05f, 0.1f, 0.0f, -0.05f, 0.1f, 0.0f}
};

void AxSpearManState3Consecutive::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	attack_ = axSpearMan_->GetAttack();

	// 媒介変数
	parameter_ = 0.0f;

	// フレーム数
	periodFrame_ = static_cast<uint32_t>(kConstAttak.time_[kPosture1]);

	// フェーズ
	inPhase_ = kPosture1;

	AttackInitialize();

	axSpearManStateNo_ = AxSpearManState::kAxSpearManState3Consecutive;

	attack_->ClearContactRecord();

}

void AxSpearManState3Consecutive::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	Attack();

	AttackPhaseFinished();

	axSpearMan_->SetReceiveCommand(false);

}

void AxSpearManState3Consecutive::AttackInitialize()
{

	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	periodFrame_ = kConstAttak.time_[inPhase_];

	//モーションネーム
	axSpearManMotionNo_ = kAxSpearManMotion3Consecutive;

	attack_->SetCenter(attackCenter_);
	attack_->SetRadius(attackRadius_);

}

void AxSpearManState3Consecutive::Attack()
{

	Move();

	// コライダー更新
	bool isAttack = (inPhase_ == static_cast<uint32_t>(ComboPhase::kAttack1)) || 
			(inPhase_ == static_cast<uint32_t>(ComboPhase::kAttack2)) ||
			(inPhase_ == static_cast<uint32_t>(ComboPhase::kAttack3));

	if (isAttack && parameter_ >= 0.5f) {
		attack_->Update();
		attack_->SetIsAttackJudgment(true);
	}
	else {
		DontAttack();
	}

}

void AxSpearManState3Consecutive::DontAttack()
{

	attack_->Stop();

}

void AxSpearManState3Consecutive::Move()
{


	WorldTransform* worldTransform = axSpearMan_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	Player* player = axSpearMan_->GetPlayer();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	WorldTransform* playerWorldTransform = player->GetWorldTransformAdress();

	Vector3 direction = worldTransform->direction_;
	// 向き

	direction = Vector3::Normalize(Vector3::Subtract(playerWorldTransform->GetWorldPosition(), worldTransform->GetWorldPosition()));

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };

	// 向き
	Vector3 targetDirection = Vector3::Multiply(std::fabsf(kConstAttak.speed_[inPhase_]), Vector3::Normalize(move));
	// 移動量に速さを反映
	move = Vector3::Multiply(kConstAttak.speed_[inPhase_], Vector3::Normalize(move));

	Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, direction);

	// 移動ベクトルをカメラの角度だけ回転する
	move = Matrix4x4::TransformNormal(move, rotateMatrix);
	targetDirection = Matrix4x4::TransformNormal(targetDirection, rotateMatrix);

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	targetDirection_.x = Vector3::Normalize(targetDirection).x;
	targetDirection_.z = Vector3::Normalize(targetDirection).z;

	// 角度補間
	worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);

	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

}

void AxSpearManState3Consecutive::AttackPhaseFinished()
{

	if (parameter_ >= 1.0f) {
		inPhase_++;
		AttackInitialize();
		if (inPhase_ == static_cast<uint32_t>(ComboPhase::kCountOfComboPhase)) {
			axSpearManStateNo_ = AxSpearManState::kAxSpearManStateRoot;
			DontAttack();
		} else if(inPhase_ == static_cast<uint32_t>(ComboPhase::kRecovery1) || 
			inPhase_ == static_cast<uint32_t>(ComboPhase::kRecovery2)){
			
			attack_->ClearContactRecord();
		
		}
		else {
			DontAttack();
		}
	}

}
