#include "EnemyAttack.h"
#include "../../Player/Player.h"

void EnemyAttack::Initialize(WorldTransform* parent)
{

	// あたり判定コライダー
	collider_ = std::make_unique<Capsule>();

	// あたり判定ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

	// 攻撃球の半径
	radius_ = 2.0f;

	// 攻撃球と手の距離
	length_ = { 0.0f, 0.0f, 1.0f };

	// 攻撃球のプレイヤーからのローカル位置
	center_ = { -10000.0f,-10000.0f,-10000.0f };

	// 前フレームの攻撃球
	prevCenter_ = { -10000.0f,-10000.0f,-10000.0f };

	// あたり判定コライダー初期化
	collider_->Initialize(Segment{ center_ , {0.0f,0.0f,0.0f} }, radius_, this);
	collider_->SetCollisionAttribute(0x00000002);
	collider_->SetCollisionMask(0xfffffffd);

	// あたり判定を取るか
	isAttackJudgment_ = false;

}

void EnemyAttack::Update()
{

	worldTransform_.transform_.translate = length_;
	worldTransform_.UpdateMatrix();
	if (center_.x <= -10000.0f) {
		prevCenter_ = worldTransform_.GetWorldPosition();
	}
	else {
		prevCenter_ = center_;
	}
	center_ = worldTransform_.GetWorldPosition();
	Segment segment;
	segment.origin_ = center_;
	segment.diff_ = Vector3::Subtract(prevCenter_, center_);
	collider_->segment_ = segment;
	collider_->radius_ = radius_;

}

void EnemyAttack::Stop()
{

	isAttackJudgment_ = false;
	center_ = { -10000.0f,-10000.0f,-10000.0f };
	prevCenter_ = { -10000.0f,-10000.0f,-10000.0f };
	collider_->segment_.origin_ = center_;
	collider_->segment_.diff_ = { 0.0f, 0.0f, 0.0f };
	collider_->worldTransformUpdate();

}

void EnemyAttack::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Player*>(colliderPartner)) {
		OnCollisionPlayer(colliderPartner, collisionData);
	}

}

void EnemyAttack::ClearContactRecord()
{

	contactRecord_.Clear();

}

void EnemyAttack::OnCollisionPlayer(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	Player* player = std::get<Player*>(colliderPartner);
	uint32_t serialNumber = player->GetSerialNumber();

	// プレイヤーが回避か
	if (player->GetCurrentStateNo() == kPlayerStateAvoidance) {
		return;
	}

	// 履歴確認
	if (contactRecord_.ConfirmHistory(serialNumber)) {
		return;
	}

	// 履歴登録
	contactRecord_.AddHistory(serialNumber);

	// 衝突処理
	player->Damage(damage_);

	EulerTransform transform = { { 5.0f, 5.0f, 5.0f },{ 0.0f, 0.0f, 0.0f}, player->GetWorldTransformAdress()->GetWorldPosition() };
	ParticleManager::GetInstance()->MakeEmitter(transform, 3, 0.005f, 0.2f, ParticleModelIndex::kCircle, ParticleName::kBloadParticle, 0);

}
