#include "BaseWeapon.h"
#include "../Ground/Ground.h"
#include "../../../Engine/Physics/InertiaTensor.h"
#include "../../Engine/Math/DeltaTime.h"
#include "../../Engine/Physics/Gravity.h"

void BaseWeapon::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	// 抵抗
	coefficientOfRestitution = 0.8f;

}

void BaseWeapon::Update()
{

	// 親あり更新
	if (parentMatrix_) {
		WithParentsUpdate();
	}
	// 親なし更新
	else {
		ParentlessUpdate();
	}

	// コライダーの更新
	ColliderUpdate();

}

void BaseWeapon::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Ground*>(colliderPartner)) {
		OnCollisionGround(colliderPartner, collisionData);
	}

}

void BaseWeapon::CollisionListRegister(CollisionManager* collisionManager)
{

	if (!parentMatrix_) {
		MeshObject::CollisionListRegister(collisionManager);
	}

}

void BaseWeapon::CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	if (!parentMatrix_) {
		MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);
	}

}

void BaseWeapon::RigidBodyInitialize()
{

	// 慣性テンソル作成
	rigidBody_.inertiaTensor = InertiaTensor::CreateRectangular(0.01f, Vector3{ 1.0f, 6.0f, 1.0f });

	// 基本姿勢での慣性テンソル作成
	rigidBody_.basicPostureInertiaTensor = InertiaTensor::CreateRectangular(0.01f, Vector3{ 1.0f, 6.0f, 1.0f });

	// 姿勢行列作成
	rigidBody_.postureMatrix = Matrix4x4::MakeRotateXYZMatrix({ 0.0f, 0.0f, 0.0f });

	rigidBody_.angularVelocity = { 0.0f,0.0f,0.0f }; // 角速度
	rigidBody_.angularMomentum = { 0.0f,0.0f,0.0f }; // 角運動量

	rigidBody_.centerOfGravityVelocity = { 0.0f,0.0f,0.0f }; // 重心位置速度

}

void BaseWeapon::RigidBodyUpdate()
{

	// 重力

	rigidBody_.centerOfGravityVelocity += Gravity::Execute();

	// 速度算出
	Vector3 velocity = RigidBody::PointVelocityCalc(
		rigidBody_.angularVelocity,
		rigidBody_.centerOfGravityVelocity,
		worldTransform_.GetWorldPosition(),
		rigidBody_.centerOfGravity
	);

	// 速度更新
	worldTransform_.transform_.translate += velocity * kDeltaTime_;

	// 姿勢行列を更新
	rigidBody_.postureMatrix = RigidBody::PostureCalc(rigidBody_.postureMatrix, rigidBody_.angularVelocity, kDeltaTime_);

	// 慣性テンソルを更新
	rigidBody_.inertiaTensor = RigidBody::InertiaTensorCalc(rigidBody_.postureMatrix, rigidBody_.basicPostureInertiaTensor);

	// 角運動量を更新
	rigidBody_.angularMomentum = RigidBody::AngularMomentumCalc(rigidBody_.angularMomentum, rigidBody_.torque, kDeltaTime_);

	// 角速度を更新
	rigidBody_.angularVelocity = RigidBody::AngularVelocityCalc(rigidBody_.inertiaTensor, rigidBody_.angularMomentum);

	// 角速度の制御処理
	const float kMaxAngularVelocity = 10.0f;
	if (std::fabsf(rigidBody_.angularVelocity.x) > kMaxAngularVelocity) {
		rigidBody_.angularVelocity.x = rigidBody_.angularVelocity.x / std::fabsf(rigidBody_.angularVelocity.x) * kMaxAngularVelocity;
	}

	if (std::fabsf(rigidBody_.angularVelocity.y) > kMaxAngularVelocity) {
		rigidBody_.angularVelocity.y = rigidBody_.angularVelocity.y / std::fabsf(rigidBody_.angularVelocity.y) * kMaxAngularVelocity;
	}

	if (std::fabsf(rigidBody_.angularVelocity.z) > kMaxAngularVelocity) {
		rigidBody_.angularVelocity.z = rigidBody_.angularVelocity.z / std::fabsf(rigidBody_.angularVelocity.z) * kMaxAngularVelocity;
	}

	// 抵抗
	const float kResistAngularVelocity = 0.1f;

	if (rigidBody_.angularVelocity.x != 0.0f) {
		rigidBody_.angularVelocity.x =
			rigidBody_.angularVelocity.x / std::fabsf(rigidBody_.angularVelocity.x)
			* std::fmaxf(0.0f, std::fabsf(rigidBody_.angularVelocity.x) - kResistAngularVelocity);
	}

	if (rigidBody_.angularVelocity.y != 0.0f) {
		rigidBody_.angularVelocity.y =
			rigidBody_.angularVelocity.y / std::fabsf(rigidBody_.angularVelocity.y)
			* std::fmaxf(0.0f, std::fabsf(rigidBody_.angularVelocity.y) - kResistAngularVelocity);
	}

	if (rigidBody_.angularVelocity.z != 0.0f) {
		rigidBody_.angularVelocity.z =
			rigidBody_.angularVelocity.z / std::fabsf(rigidBody_.angularVelocity.z)
			* std::fmaxf(0.0f, std::fabsf(rigidBody_.angularVelocity.z) - kResistAngularVelocity);
	}

	// ひねり力を0に
	rigidBody_.torque = { 0.0f,0.0f,0.0f };

	// ワールドトランスフォーム更新
	worldTransform_.UpdateMatrix(rigidBody_.postureMatrix);

}

void BaseWeapon::OnCollisionGround(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	// 地面情報取得
	Ground* ground = std::get<Ground*>(colliderPartner);
	OBB groundOBB = std::get<OBB>(*ground->GetCollider());

	OBB obb = std::get<OBB>(*GetCollider());

	// 地面に近い点を2点求める
	Vector3 obbVertex[8];

	obbVertex[0] = {
		+obb.size_.x,
		+obb.size_.y,
		+obb.size_.z };

	obbVertex[1] = {
		+obb.size_.x,
		+obb.size_.y,
		-obb.size_.z };

	obbVertex[2] = {
		+obb.size_.x,
		-obb.size_.y,
		+obb.size_.z };

	obbVertex[3] = {
		+obb.size_.x,
		-obb.size_.y,
		-obb.size_.z };

	obbVertex[4] = {
		-obb.size_.x,
		+obb.size_.y,
		+obb.size_.z };

	obbVertex[5] = {
		-obb.size_.x,
		+obb.size_.y,
		-obb.size_.z };

	obbVertex[6] = {
		-obb.size_.x,
		-obb.size_.y,
		+obb.size_.z };

	obbVertex[7] = {
		-obb.size_.x,
		-obb.size_.y,
		-obb.size_.z };

	Matrix4x4 obbRotateMatrix = {
		obb.otientatuons_[0].x,obb.otientatuons_[0].y,obb.otientatuons_[0].z,0.0f,
		obb.otientatuons_[1].x,obb.otientatuons_[1].y,obb.otientatuons_[1].z,0.0f,
		obb.otientatuons_[2].x,obb.otientatuons_[2].y,obb.otientatuons_[2].z,0.0f,
		0.0f,0.0f,0.0f,1.0f };

	for (uint32_t i = 0; i < 8; i++) {

		obbVertex[i] = Matrix4x4::Transform(obbVertex[i], obbRotateMatrix);
		obbVertex[i] = Vector3::Add(obbVertex[i], obb.center_);

	}

	// 保存用
	float minY = 0.0f;
	uint32_t number = 0;
	float minY2 = 0.0f;
	uint32_t number2 = 0;

	if (obbVertex[0].y > obbVertex[1].y) {
		minY = obbVertex[1].y;
		number = 1;
		minY2 = obbVertex[0].y;
		number2 = 0;
	}
	else {
		minY = obbVertex[0].y;
		number = 0;
		minY2 = obbVertex[1].y;
		number2 = 1;
	}

	for (uint32_t i = 2; i < 8; i++) {
		
		if (minY > obbVertex[i].y) {
			minY2 = minY;
			number2 = number;
			minY = obbVertex[i].y;
			number = i;
		}
		else if (minY2 > obbVertex[i].y) {
			minY2 = obbVertex[i].y;
			number2 = i;
		}
	
	}

	// 力を加える
	const Vector3 force = { 0.0f, 100.0f, 0.0f };
	ApplyForce((obbVertex[number] + obbVertex[number2]) * 0.5f, force);
	
	// 反発
	rigidBody_.centerOfGravityVelocity = rigidBody_.centerOfGravityVelocity * -coefficientOfRestitution;

	// 押し出し
	OBB groundObb = std::get<OBB>(*ground->GetCollider());
	float sub = ground->GetWorldTransformAdress()->GetWorldPosition().y + groundObb.size_.y - obbVertex[number].y;
	worldTransform_.transform_.translate.y += sub;
	worldTransform_.UpdateMatrix(rigidBody_.postureMatrix);

}

void BaseWeapon::ApplyForce(const Vector3& pointOfAction, const Vector3& force)
{

	OBB obb = std::get<OBB>(*GetCollider());

	rigidBody_.centerOfGravity = obb.center_;
	rigidBody_.torque = RigidBody::TorqueCalc(rigidBody_.centerOfGravity, pointOfAction, force);

}
