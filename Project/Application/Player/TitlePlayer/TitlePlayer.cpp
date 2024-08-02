#include "TitlePlayer.h"
#include "../../../Engine/3D/ModelDraw.h"

void TitlePlayer::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	material_->SetEnableLighting(BlinnPhongReflection);
	material_->SetEnvironmentCoefficient(0.005f);

	EulerTransform uvTransform;
	uvTransform.scale = { 100.0f, 100.0f,1.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.translate = { 0.0f,0.0f,0.0f };
	material_->SetUvTransform(uvTransform);

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// アニメーション
	animation_.StartAnimation(5, true);

	for (uint32_t i = 0; i < 20; ++i) {
		animation_.AnimationUpdate();
	}

}

void TitlePlayer::Update()
{

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	localMatrixManager_->Map();

	worldTransform_.UpdateMatrix();

}

void TitlePlayer::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void TitlePlayer::ImGuiDraw()
{
}

void TitlePlayer::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}
