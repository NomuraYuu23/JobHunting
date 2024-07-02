#include "BaseEnemyAttack.h"
#include "../Player/Player.h"

void BaseEnemyAttack::Initialize(WorldTransform* parent)
{

	// あたり判定コライダー
	collider_ = std::make_unique<Sphere>();

	// あたり判定ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

	// 攻撃球の半径
	radius_ = 2.0f;

	// 攻撃球のプレイヤーからのローカル位置
	center_ = { -10000.0f,-10000.0f,-10000.0f };

	// あたり判定コライダー初期化
	collider_->Initialize(center_, radius_, this);
	collider_->SetCollisionAttribute(0x00000001);
	collider_->SetCollisionMask(0xfffffffe);

	// あたり判定を取るか
	isAttackJudgment_ = false;

}

void BaseEnemyAttack::Update()
{

	worldTransform_.transform_.translate = center_;
	worldTransform_.UpdateMatrix();

	Vector3 center = worldTransform_.GetWorldPosition();
	collider_->center_ = center;
	collider_->radius_ = radius_;

}

void BaseEnemyAttack::Stop()
{

	isAttackJudgment_ = false;
	center_ = { -10000.0f,-10000.0f,-10000.0f };
	collider_->worldTransformUpdate();

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
