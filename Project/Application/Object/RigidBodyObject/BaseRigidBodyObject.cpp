#include "BaseRigidBodyObject.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Physics/Gravity.h"
#include "../../../Engine/Collision/Extrusion.h"
#include "../../Collider/ColliderParentIdentification.h"
#include "../Obstacle/Ground/Ground.h"

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

	std::string name = pair->GetName();
	size_t len = name.length();
	size_t find = name.find("Ground");
	find = static_cast<size_t>(std::fminf(static_cast<float>(len), static_cast<float>(find)));
	bool isGround = (find != len);

	// 力設定
	Vector3 pairVelocity = pair->GetSaveVelocity();
	Vector3 velocity = saveVelocity_;
	Vector3 force = {0.0f, 0.0f, 0.0f};

	// 速度方向が逆か？
	if (velocity.x * pairVelocity.x <= 0.0f) {
		force.x = -velocity.x + pairVelocity.x;
	}
	if (velocity.y * pairVelocity.y <= 0.0f) {
		force.y = -velocity.y + pairVelocity.y;
	}
	if (velocity.z * pairVelocity.z <= 0.0f) {
		force.z = -velocity.z + pairVelocity.z;
	}

	// 値が小さいので100倍しとく
	float power = Vector3::Length(force) * 50.0f;

	RigidBody::CollisionPositionConfirmation(&rigidBody_, obb, pairOBB, coefficientOfRestitution, isGround, power);

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
	const float kMaxAngularVelocity = 1.0f;
	
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
	const float kResistAngularVelocity = 0.5f;

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

	//centerOfGravityVelocity抵抗付けてみる

	// ひねり力を0に
	rigidBody_.torque = { 0.0f,0.0f,0.0f };

	// ワールドトランスフォーム更新
	worldTransform_.UpdateMatrix(rigidBody_.postureMatrix);

}
