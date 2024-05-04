#include "PlayerAttack.h"
#include "../../Enemy/Enemy.h"
#include "../../../Engine/Particle/ParticleManager.h"
#include "../../TutorialEnemy/TutorialEnemy.h"

void PlayerAttack::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Enemy*>(colliderPartner)) {
		OnCollisionEnemy(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<TutorialEnemy*>(colliderPartner)) {
		OnCollisionTutorialEnemy(colliderPartner, collisionData);
	}
}

void PlayerAttack::ClearContactRecord()
{

	contactRecord_.Clear();

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
	ParticleManager::GetInstance()->MakeEmitter(transform, 3, 0.005f, 0.1f, ParticleModelIndex::kCircle, ParticleName::kBloadParticle, 0);

}

void PlayerAttack::OnCollisionTutorialEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	TutorialEnemy* enemy = std::get<TutorialEnemy*>(colliderPartner);
	uint32_t serialNumber = enemy->GetSerialNumber();

	// 履歴確認
	if (contactRecord_.ConfirmHistory(serialNumber)) {
		return;
	}

	// 履歴登録
	contactRecord_.AddHistory(serialNumber);

	EulerTransform transform = { { 5.0f, 5.0f, 5.0f },{ 0.0f, 0.0f, 0.0f}, enemy->GetWorldTransformAdress()->GetWorldPosition() };
	ParticleManager::GetInstance()->MakeEmitter(transform, 3, 0.005f, 0.1f, ParticleModelIndex::kCircle, ParticleName::kBloadParticle, 0);

}
