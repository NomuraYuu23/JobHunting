#include "BossFog.h"

void BossFog::Initialize(LevelData::MeshData* data)
{

	BaseObstacle::Initialize(data);

	EulerTransform uvTransform;
	uvTransform.scale = { worldTransform_.transform_.scale.x / 2.0f, worldTransform_.transform_.scale.z / 2.0f,1.0f };
	uvTransform.rotate = { 0.0f,0.0f,0.0f };
	uvTransform.translate = { 0.0f,0.0f,0.0f };
	material_->SetUvTransform(uvTransform);

	battleTranslate_ = worldTransform_.transform_.translate;
	translate_ = { -10000.0f, -10000.0f, -10000.0f };

	bossSystem_ = nullptr;

}

void BossFog::Update()
{

	if (!bossSystem_) {
		assert(0);
	}

	BaseObstacle::Update();

	if (bossSystem_->GetIsBossBattle()) {
		worldTransform_.transform_.translate = battleTranslate_;
	}
	else {
		worldTransform_.transform_.translate = translate_;
	}

	worldTransform_.UpdateMatrix();

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}
