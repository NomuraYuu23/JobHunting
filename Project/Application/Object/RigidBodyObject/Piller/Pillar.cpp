#include "Pillar.h"
#include "../../Character/Enemy/BaseEnemyAttack.h"
#include "../../Character/Player/PlayerAttack/PlayerAttack.h"
#include "../../../../Engine/Math/DeltaTime.h"

void Pillar::Initialize(LevelData::MeshData* data)
{

	BaseRigidBodyObject::Initialize(data);
	// 壊れたか
	broken_ = false;

	// 耐久値
	durable_ = kInitDurable_;

	coefficientOfRestitution = 0.1f;

	// パーティクル
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	pillarSmokeParticle_ = std::make_unique<PillarSmokeParticle>();

	pillarSmokeParticle_->Initialize(
		dxCommon->GetDevice(),
		dxCommon->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexGPUParticleDissolve].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexGPUParticleDissolve].Get());

	EmitterCS emitter;
	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.frequencyTime = 0.0f;
	emitter.translate = worldTransform_.GetWorldPosition();
	emitter.radius = 1.0f;
	emitter.emit = 0;

	pillarSmokeParticle_->SetEmitter(emitter);

	particleTime_ = particleTimeMax_;

}

void Pillar::Update()
{

	MeshObject::Update();

	if (broken_) {
		BaseRigidBodyObject::Update();
	}
	else {
		worldTransform_.UpdateMatrix();
	}

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.center_ += Matrix4x4::TransformNormal(colliderAddPos,worldTransform_.rotateMatrix_);
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

	// 速度保存
	SaveVelocityUpdate();

	// パーティクル
	ParticleUpdate();

}

void Pillar::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (broken_) {
		BaseRigidBodyObject::OnCollision(colliderPartner, collisionData);
	}
	else if(std::holds_alternative<BaseEnemyAttack*>(colliderPartner)){

		BaseEnemyAttack* attack = std::get<BaseEnemyAttack*>(colliderPartner);

		// 履歴確認
		if (attack->GetContactRecord().ConfirmHistory(serialNumber_)) {
			return;
		}

		// 履歴登録
		attack->GetContactRecord().AddHistory(serialNumber_);

		// 衝突処理
		Damage(1, colliderPartner);

	}
	else if (std::holds_alternative<PlayerAttack*>(colliderPartner)) {

		PlayerAttack* attack = std::get<PlayerAttack*>(colliderPartner);

		// 履歴確認
		if (attack->GetContactRecord().ConfirmHistory(serialNumber_)) {
			return;
		}

		// 履歴登録
		attack->GetContactRecord().AddHistory(serialNumber_);

		// 衝突処理
		Damage(1, colliderPartner);

	}

}

void Pillar::ParticleUpdate()
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

		pillarSmokeParticle_->SetEmitter(emitter);

	}
	else {
		pillarSmokeParticle_->Update();
	}


}

void Pillar::ParticleDraw(BaseCamera& camera)
{

	pillarSmokeParticle_->Draw(commandList_, camera);

}

void Pillar::Damage(uint32_t damage, ColliderParentObject colliderPartner)
{

	durable_ -= damage;

	// パーティクル
	EmitterCS emitter;
	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.frequencyTime = 0.0f;
	emitter.translate = worldTransform_.GetWorldPosition();
	emitter.radius = 1.0f;
	emitter.emit = 0;

	if (durable_ <= 0) {
	
		OBB pairOBB;

		broken_ = true;
		worldTransform_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransform_.parent_ = nullptr;

		OBB obb = std::get<OBB>(*GetCollider());
		Vector3 attackPos = {};
		if (std::holds_alternative<BaseEnemyAttack*>(colliderPartner)) {

			BaseEnemyAttack* attack = std::get<BaseEnemyAttack*>(colliderPartner);
			attackPos = attack->GetParentPos();
			

		}
		else if (std::holds_alternative<PlayerAttack*>(colliderPartner)) {

			PlayerAttack* attack = std::get<PlayerAttack*>(colliderPartner);
			attackPos = attack->GetParentPos();

		}
		else {
			assert(0);
		}

		Vector3 myPos = worldTransform_.GetWorldPosition();
		attackPos.y = 0.0f;
		myPos.y = 0.0f;
		Vector3 dir = Vector3::Normalize(myPos - attackPos);
		float power = 150.0f;
		if (dir.x == 0.0f && dir.z == 0.0f) {
			dir.z = 0.0f;
			dir.x = power;
		}
		else if (std::fabsf(dir.x) > std::fabsf(dir.z)) {
			dir.z = 0.0f;
			dir.x = power * (dir.x / std::fabsf(dir.x));
		}
		else {
			dir.z = power * (dir.z / std::fabsf(dir.z));
			dir.x = 0.0f;
		}

		rigidBody_.ApplyForce(obb.center_, obb.center_ + colliderAddPos, dir);

		float centerOfGravityVelocityPower = 2.5f;
		rigidBody_.centerOfGravityVelocity = Vector3::Normalize(dir) * centerOfGravityVelocityPower;

		emitter.count = 30;

	}

	// パーティクル
	pillarSmokeParticle_->SetEmitter(emitter);
	particleTime_ = 0.0f;

}

void Pillar::SetParent(PillarFoundation* parent)
{

	parent_ = parent;

	//土台分
	worldTransform_.transform_.translate = {0.0f,2.0f,0.0f};
	worldTransform_.parent_ = parent_->GetWorldTransformAdress();

}
