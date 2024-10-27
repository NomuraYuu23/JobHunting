#include "BaseEnemy.h"
#include "../../../Engine/Animation/LocalMatrixDraw.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/Physics/RigidBody/InertiaTensor.h"
#include "../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"

void BaseEnemy::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	//衝突属性(自分)
	collisionAttribute_ = 0x00000002;
	// 衝突マスク(相手)
	collisionMask_ = 0xfffffffd;

	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
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

	usedRigidBody_ = false;

	coefficientOfRestitution = 0.8f;

	height_ = 0.0f;

}

void BaseEnemy::Update()
{

	MeshObject::Update();

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

void BaseEnemy::ParticleDraw(BaseCamera& camera)
{

}

void BaseEnemy::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	// 地面情報取得
	BaseObstacle* obstacle = std::get<BaseObstacle*>(colliderPartner);
	OBB obstacleOBB = std::get<OBB>(*obstacle->GetCollider());

	OBB obb = std::get<OBB>(*GetCollider());

	if (usedRigidBody_) {

		std::string name = obstacle->GetName();
		size_t len = name.length();
		size_t find = name.find("Ground");
		find = static_cast<size_t>(std::fminf(static_cast<float>(len), static_cast<float>(find)));
		bool isGround = (find != len);

		RigidBody::CollisionPositionConfirmation(&rigidBody_, obb, obstacleOBB, coefficientOfRestitution, isGround, 10.0f);

		Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*collider_), &obstacleOBB);

		worldTransform_.transform_.translate += extrusion;
		worldTransform_.UpdateMatrix(rigidBody_.postureMatrix);
	}
	else {
		Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*collider_), &std::get<OBB>(*obstacle->GetCollider()));

		worldTransform_.transform_.translate += extrusion;
		worldTransform_.UpdateMatrix();
	}

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
