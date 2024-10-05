#include "GhostStateCadaver.h"
#include "Ghost.h"

#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/Math/DeltaTime.h"

void GhostStateCadaver::Initialize()
{

	ghostStateNo_ = GhostState::kGhostStateCadaver;

	ghostMotionNo_ = kGhostMotionCadaver;

	// コライダー調整
	OBB obb = std::get<OBB>(*ghost_->GetCollider());

	float tmp = obb.size_.y;
	obb.size_.y = obb.size_.x;
	obb.size_.x = tmp;
	obb.size_.z = tmp;

	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	ghost_->SetCollider(colliderShape);

	// 剛体ちから入れる
	RigidBody* rigidBody = ghost_->GetRigidBody();

	//テンソルを作り直す
	rigidBody->Initialize(0.01f, obb.size_);

	rigidBody->ApplyForce(obb.center_, rigidBody->centerOfGravity + Vector3{ 1.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 50.0f, 0.0f });
	rigidBody->centerOfGravityVelocity.y = 10.0f;

	ghost_->SetUsedRigidBody(true);

}

void GhostStateCadaver::Update()
{

	RigidBodyUpdate();
	ghost_->SetReceiveCommand(false);

	alphaChangeStartTimeCount += kDeltaTime_;

	if (alphaChangeStartTimeCount >= alphaChangeStartTime) {
		alphaChangeStartTimeCount = alphaChangeStartTime;

		alpha -= alphaSpeed;

		if (alpha <= 0.0f) {
			alpha = 0.0f;
			ghost_->SetIsDead(true);
		}

		Vector4 color = { 1.0f, 1.0f, 1.0f, alpha };
		ghost_->SetMaterialColor(color);

	}

}

void GhostStateCadaver::RigidBodyUpdate()
{

	RigidBody* rigidBody = ghost_->GetRigidBody();

	WorldTransform* worldTransform = ghost_->GetWorldTransformAdress();

	// 重力

	rigidBody->centerOfGravityVelocity += Gravity::Execute();

	rigidBody->centerOfGravity = worldTransform->GetWorldPosition();

	// 速度算出
	Vector3 velocity = RigidBody::PointVelocityCalc(
		rigidBody->angularVelocity,
		rigidBody->centerOfGravityVelocity,
		worldTransform->GetWorldPosition(),
		rigidBody->centerOfGravity
	);

	// 速度更新
	worldTransform->transform_.translate += velocity * kDeltaTime_;

	// 姿勢行列を更新
	rigidBody->postureMatrix = RigidBody::PostureCalc(rigidBody->postureMatrix, rigidBody->angularVelocity, kDeltaTime_);

	// 慣性テンソルを更新
	rigidBody->inertiaTensor = RigidBody::InertiaTensorCalc(rigidBody->postureMatrix, rigidBody->basicPostureInertiaTensor);

	// 角運動量を更新
	rigidBody->angularMomentum = RigidBody::AngularMomentumCalc(rigidBody->angularMomentum, rigidBody->torque, kDeltaTime_);

	// 角速度を更新
	rigidBody->angularVelocity = RigidBody::AngularVelocityCalc(rigidBody->inertiaTensor, rigidBody->angularMomentum);

	// 角速度の制御処理
	const float kMaxAngularVelocity = 10.0f;
	if (std::fabsf(rigidBody->angularVelocity.x) > kMaxAngularVelocity) {
		rigidBody->angularVelocity.x = rigidBody->angularVelocity.x / std::fabsf(rigidBody->angularVelocity.x) * kMaxAngularVelocity;
	}

	if (std::fabsf(rigidBody->angularVelocity.y) > kMaxAngularVelocity) {
		rigidBody->angularVelocity.y = rigidBody->angularVelocity.y / std::fabsf(rigidBody->angularVelocity.y) * kMaxAngularVelocity;
	}

	if (std::fabsf(rigidBody->angularVelocity.z) > kMaxAngularVelocity) {
		rigidBody->angularVelocity.z = rigidBody->angularVelocity.z / std::fabsf(rigidBody->angularVelocity.z) * kMaxAngularVelocity;
	}

	// 抵抗
	const float kResistAngularVelocity = 1.0f;

	if (rigidBody->angularVelocity.x != 0.0f) {
		rigidBody->angularVelocity.x =
			rigidBody->angularVelocity.x / std::fabsf(rigidBody->angularVelocity.x)
			* std::fmaxf(0.0f, std::fabsf(rigidBody->angularVelocity.x) - kResistAngularVelocity);
	}

	if (rigidBody->angularVelocity.y != 0.0f) {
		rigidBody->angularVelocity.y =
			rigidBody->angularVelocity.y / std::fabsf(rigidBody->angularVelocity.y)
			* std::fmaxf(0.0f, std::fabsf(rigidBody->angularVelocity.y) - kResistAngularVelocity);
	}

	if (rigidBody->angularVelocity.z != 0.0f) {
		rigidBody->angularVelocity.z =
			rigidBody->angularVelocity.z / std::fabsf(rigidBody->angularVelocity.z)
			* std::fmaxf(0.0f, std::fabsf(rigidBody->angularVelocity.z) - kResistAngularVelocity);
	}

	// ひねり力を0に
	rigidBody->torque = { 0.0f,0.0f,0.0f };

	// ワールドトランスフォーム更新
	worldTransform->UpdateMatrix(rigidBody->postureMatrix);

}