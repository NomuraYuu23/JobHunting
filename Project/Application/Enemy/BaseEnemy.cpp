#include "BaseEnemy.h"
#include "../../Engine/Animation/LocalMatrixDraw.h"
#include "../../Engine/3D/ModelDraw.h"
#include "../../Engine/Physics/InertiaTensor.h"

#include "../Ground/Ground.h"

void BaseEnemy::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	attack_ = std::make_unique<BaseEnemyAttack>();
	attack_->Initialize(&worldTransform_);

	// コマンド
	CommandInitialize();

	// ステート
	StateInitialize();

	// パーツ
	PartInitialize();

	//HP
	HPInit(1);

	// 剛体初期化
	obb = std::get<OBB>(*collider_.get());
	rigidBody_.Initialize(0.01f, obb.size_);

	coefficientOfRestitution = 0.8f;

}

void BaseEnemy::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void BaseEnemy::DebugDrawMap(DrawLine* drawLine)
{

	LocalMatrixDraw::DrawMap(
		localMatrixManager_.get(),
		&worldTransform_,
		drawLine);

}

void BaseEnemy::CollisionListRegister(CollisionManager* collisionManager)
{

	MeshObject::CollisionListRegister(collisionManager);

	attack_->CollisionListRegister(collisionManager);

}

void BaseEnemy::CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);

	attack_->CollisionListRegister(collisionManager, colliderDebugDraw);

}

void BaseEnemy::OnCollisionGround(ColliderParentObject colliderPartner, const CollisionData& collisionData)
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
	//Vector3 force = (obb.center_ - (obbVertex[number] + obbVertex[number2]) * 0.5f);
	//force.x = 0.0f;
	//force.z = 0.0f;
	Vector3 force = { 0.0f,10.0f,0.0f };

	rigidBody_.ApplyForce(obb.center_, (obbVertex[number] + obbVertex[number2]) * 0.5f, force);

	// 反発
	rigidBody_.centerOfGravityVelocity = rigidBody_.centerOfGravityVelocity * -coefficientOfRestitution;

	// 押し出し
	OBB groundObb = std::get<OBB>(*ground->GetCollider());
	float sub = ground->GetWorldTransformAdress()->GetWorldPosition().y + groundObb.size_.y - obbVertex[number].y;
	worldTransform_.transform_.translate.y += sub;
	worldTransform_.UpdateMatrix(rigidBody_.postureMatrix);

}

void BaseEnemy::HPInit(uint32_t initHp)
{

	initHp_ = initHp;
	hp_ = initHp;

	isDead_ = false;

}

void BaseEnemy::Damage(uint32_t damage)
{

	hp_ -= damage;

}

float BaseEnemy::RatioHP()
{

	assert(initHp_ != 0);

	return static_cast<float>(hp_) / static_cast<float>(initHp_);

}
