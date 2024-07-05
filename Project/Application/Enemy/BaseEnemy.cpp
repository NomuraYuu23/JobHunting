#include "BaseEnemy.h"
#include "../../Engine/Animation/LocalMatrixDraw.h"
#include "../../Engine/3D/ModelDraw.h"

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

void BaseEnemy::HPInit(uint32_t initHp)
{

	initHp_ = initHp;
	hp_ = initHp;

	isDead_ = false;

}

void BaseEnemy::Damage(uint32_t damage)
{

	hp_ -= damage;

	if (hp_ <= 0) {
		isDead_ = true;
	}

}

float BaseEnemy::RatioHP()
{

	assert(initHp_ != 0);

	return static_cast<float>(hp_) / static_cast<float>(initHp_);

}
