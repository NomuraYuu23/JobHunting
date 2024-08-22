#include "PlayerAttack.h"
#include "../../Enemy/BaseEnemy.h"
#include "../../../Engine/Particle/ParticleManager.h"
#include "../../../Engine/Particle/EmitterDesc.h"
#include "../../../../../Engine/Math/DeltaTime.h"

void PlayerAttack::Initialize(WorldTransform* parent)
{

	// あたり判定ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

	// 攻撃球の半径
	radius_ = 2.0f;

	// 攻撃球のプレイヤーからのローカル位置
	center_ = { 0.0f, 1.5f, 3.0f };

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

	// パーティクル
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	bloadParticle_ = std::make_unique<BloadParticle>();

	bloadParticle_->Initialize(
		dxCommon->GetDevice(),
		dxCommon->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get());

	EmitterCS emitter;
	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.frequencyTime = 0.0f;
	emitter.translate = worldTransform_.GetWorldPosition();
	emitter.radius = 1.0f;
	emitter.emit = 0;

	bloadParticle_->SetEmitter(emitter);

	particleTime_ = particleTimeMax_;

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

	if (std::holds_alternative<BaseEnemy*>(colliderPartner)) {
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

void PlayerAttack::ParticleDraw(ID3D12GraphicsCommandList* commandList, BaseCamera& camera)
{

	bloadParticle_->Draw(commandList, camera);

}

void PlayerAttack::ParticleUpdate()
{
	particleTime_ += kDeltaTime_;

	if (particleTimeMax_ <= particleTime_) {
		particleTime_ = particleTimeMax_;
		EmitterCS emitter;
		emitter.count = 10;
		emitter.frequency = 0.1f;
		emitter.frequencyTime = 0.0f;
		emitter.translate = worldTransform_.GetWorldPosition();
		emitter.radius = 1.0f;
		emitter.emit = 0;

		bloadParticle_->SetEmitter(emitter);

	}
	else {
		bloadParticle_->Update();
	}

}

Vector3 PlayerAttack::GetParentPos()
{
	return worldTransform_.parent_->GetWorldPosition();
}

void PlayerAttack::OnCollisionEnemy(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	BaseEnemy* enemy = std::get<BaseEnemy*>(colliderPartner);
	uint32_t serialNumber = enemy->GetSerialNumber();

	// 履歴確認
	if (contactRecord_.ConfirmHistory(serialNumber)) {
		return;
	}

	// 履歴登録
	contactRecord_.AddHistory(serialNumber);

	// 衝突処理
	enemy->Damage(damage_);

	EmitterCS emitter;
	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.frequencyTime = 0.0f;
	emitter.translate = enemy->GetWorldTransformAdress()->GetWorldPosition();
	emitter.radius = 1.0f;
	emitter.emit = 0;

	bloadParticle_->SetEmitter(emitter);
	particleTime_ = 0.0f;

}
