#include "BaseEnemyAttack.h"
#include "../Player/Player.h"

void BaseEnemyAttack::Initialize(WorldTransform* parent)
{

	// あたり判定ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

	// 攻撃球の半径
	radius_ = 2.0f;

	// 攻撃球のプレイヤーからのローカル位置
	center_ = { -10000.0f,-10000.0f,-10000.0f };


	// あたり判定コライダー
	ColliderShape* collider = new ColliderShape();
	Sphere sphere;
	sphere.Initialize(center_, radius_, this);
	sphere.SetCollisionAttribute(0x00000002);
	sphere.SetCollisionMask(0xfffffffd);
	*collider = sphere;

	collider_.reset(collider);

	// あたり判定を取るか
	isAttackJudgment_ = false;

}

void BaseEnemyAttack::Update()
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

void BaseEnemyAttack::Stop()
{

	isAttackJudgment_ = false;
	center_ = { -10000.0f,-10000.0f,-10000.0f };
	worldTransform_.transform_.translate = center_;
	worldTransform_.UpdateMatrix();

}

void BaseEnemyAttack::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Player*>(colliderPartner)) {
		OnCollisionPlayer(colliderPartner, collisionData);
	}

}

void BaseEnemyAttack::ClearContactRecord()
{

	contactRecord_.Clear();

}

void BaseEnemyAttack::CollisionListRegister(CollisionManager* collisionManager)
{

	collisionManager->ListRegister(collider_.get());

}

void BaseEnemyAttack::CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	collisionManager->ListRegister(collider_.get());

	colliderDebugDraw->AddCollider(*collider_.get());

}

void BaseEnemyAttack::OnCollisionPlayer(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	Player* player = std::get<Player*>(colliderPartner);
	uint32_t serialNumber = player->GetSerialNumber();

	// 履歴確認
	if (contactRecord_.ConfirmHistory(serialNumber)) {
		return;
	}

	// 履歴登録
	contactRecord_.AddHistory(serialNumber);

	// 衝突処理
	player->Damage(damage_);

	// エミッタ
	EulerTransform transform = { { 5.0f, 5.0f, 5.0f },{ 0.0f, 0.0f, 0.0f}, player->GetWorldTransformAdress()->GetWorldPosition() };
	EmitterDesc emitterDesc{};
	emitterDesc.transform = &transform;
	emitterDesc.instanceCount = 3;
	emitterDesc.frequency = 0.005f;
	emitterDesc.lifeTime = 0.1f;
	emitterDesc.paeticleName = ParticleName::kBloadParticle;
	emitterDesc.particleModelNum = ParticleModelIndex::kCircle;

	ParticleManager::GetInstance()->MakeEmitter(&emitterDesc, 0);

}
