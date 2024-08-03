#include "Bonfire.h"
#include <cassert>

void Bonfire::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	material_->SetEnableLighting(HalfLambert);

	EulerTransform uvTransform;
	uvTransform.scale = { 1.0f, 1.0f,1.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.translate = { 0.0f,0.0f,0.0f };
	material_->SetUvTransform(uvTransform);

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	bonfireParticle_ = std::make_unique<BonfireParticle>();
	bonfireParticle_->Initialize(
		dxCommon->GetDevice(),
		dxCommon->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexGPUParticleDissolve].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexGPUParticleDissolve].Get());

	EmitterCS emitter;
	emitter.count = 5;
	emitter.frequency = 0.1f;
	emitter.frequencyTime = 0.0f;
	emitter.translate = worldTransform_.transform_.translate;
	emitter.radius = 0.1f;
	emitter.emit = 0;

	bonfireParticle_->SetEmitter(emitter);

}

void Bonfire::Update()
{

	bonfireParticle_->Update();

}

void Bonfire::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void Bonfire::ParticleDraw(BaseCamera& camera)
{

	assert(commandList_);

	bonfireParticle_->Draw(commandList_, camera);

}

void Bonfire::ImGuiDraw()
{
}

void Bonfire::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}
