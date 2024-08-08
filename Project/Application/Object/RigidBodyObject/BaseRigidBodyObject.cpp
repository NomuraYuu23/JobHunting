#include "BaseRigidBodyObject.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Physics/Gravity.h"
#include "../../../Engine/Collision/Extrusion.h"
#include "../../Collider/ColliderParentIdentification.h"

void BaseRigidBodyObject::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	material_->SetEnableLighting(BlinnPhongReflection);

	// 剛体初期化
	RigidBodyInitialize();

	// 抵抗
	coefficientOfRestitution = 0.8f;

}

void BaseRigidBodyObject::Update()
{

	// 剛体更新
	RigidBodyUpdate();

}

void BaseRigidBodyObject::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void BaseRigidBodyObject::ImGuiDraw()
{
}

void BaseRigidBodyObject::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	// 情報取得
	MeshObject* pair = ColliderParentIdentification::Execution(colliderPartner);

	if (!pair) {
		return;
	}

	OBB pairOBB = std::get<OBB>(*pair->GetCollider());

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


	Vector3 force = { 0.0f,10.0f,0.0f };

	rigidBody_.ApplyForce(obb.center_, (obbVertex[number] + obbVertex[number2]) * 0.5f, force);

	// 反発
	rigidBody_.centerOfGravityVelocity = rigidBody_.centerOfGravityVelocity * -coefficientOfRestitution;

	Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*collider_), &pairOBB);

	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix(rigidBody_.postureMatrix);

}

void BaseRigidBodyObject::RigidBodyInitialize()
{

	// 剛体初期化
	OBB obb = std::get<OBB>(*collider_.get());
	rigidBody_.Initialize(0.01f, obb.size_);

}

void BaseRigidBodyObject::RigidBodyUpdate()
{

	// 重力

	rigidBody_.centerOfGravityVelocity += Gravity::Execute();

	rigidBody_.centerOfGravity = worldTransform_.GetWorldPosition();

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
