#include "PlayerAttack.h"
#include "../../Enemy/Enemy.h"
#include "../../../Engine/Particle/ParticleManager.h"
#include "../../../Engine/Particle/EmitterDesc.h"

void PlayerAttack::Initialize(WorldTransform* parent)
{

	// あたり判定ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

	// 攻撃球の半径
	radius_ = 1.0f;

	// 攻撃球のプレイヤーからのローカル位置
	center_ = { 0.0f, 1.5f, 1.5f };

	// あたり判定コライダー
	ColliderShape* collider = new ColliderShape();
	Sphere sphere;
	sphere.Initialize(center_, radius_, this);
	sphere.SetCollisionAttribute(0x00000001);
	sphere.SetCollisionMask(0xfffffffe);
	*collider = sphere;

	collider_.reset(collider);

	// あたり判定を取るか
	isAttackJudgment_ = false;

}

void PlayerAttack::Update()
{

	worldTransform_.transform_.translate = center_;
	worldTransform_.UpdateMatrix();

	Vector3 center = worldTransform_.GetWorldPosition();

	Sphere sphere = std::get<Sphere>(*collider_.get());

	sphere.center_ = center;

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = sphere;

	collider_.reset(colliderShape);

}

void PlayerAttack::Stop()
{

	isAttackJudgment_ = false;

}

void PlayerAttack::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Enemy*>(colliderPartner)) {
		OnCollisionEnemy(colliderPartner, collisionData);
	}

}

void PlayerAttack::ClearContactRecord()
{

	contactRecord_.Clear();

}

void PlayerAttack::CollisionListRegister(CollisionManager* collisionManager)
{

	if (!isAttackJudgment_) {
		return;
	}

	collisionManager->ListRegister(collider_.get());

}

void PlayerAttack::CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	if (!isAttackJudgment_) {
		return;
	}

	collisionManager->ListRegister(collider_.get());

	colliderDebugDraw->AddCollider(*collider_.get());

}

void PlayerAttack::OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	Enemy* enemy = std::get<Enemy*>(colliderPartner);
	uint32_t serialNumber = enemy->GetSerialNumber();

	// 履歴確認
	if (contactRecord_.ConfirmHistory(serialNumber)) {
		return;
	}

	// 履歴登録
	contactRecord_.AddHistory(serialNumber);

	// 衝突処理
	enemy->Damage(damage_);

	EulerTransform transform = { { 5.0f, 5.0f, 5.0f },{ 0.0f, 0.0f, 0.0f}, enemy->GetWorldTransformAdress()->GetWorldPosition() };
	EmitterDesc emitterDesc{};
	emitterDesc.transform = &transform;
	emitterDesc.instanceCount = 3;
	emitterDesc.frequency = 0.005f;
	emitterDesc.lifeTime = 0.1f;
	emitterDesc.paeticleName = ParticleName::kBloadParticle;
	emitterDesc.particleModelNum = ParticleModelIndex::kCircle;

	ParticleManager::GetInstance()->MakeEmitter(&emitterDesc, 0);

}
