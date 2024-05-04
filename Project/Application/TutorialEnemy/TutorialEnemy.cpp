#include "TutorialEnemy.h"

#include "../../Engine/Collider/Capsule/Capsule.h"
#include "../../Engine/2D/ImguiManager.h"
#include "../Player/Player.h"
#include <cassert>
#include "../../Engine/3D/ModelDraw.h"

void TutorialEnemy::Initialize(Model* model)
{

	// モデル
	model_ = model;

	// マテリアル
	material_.reset(Material::Create());
	material_->SetEnableLighting(BlinnPhongReflection);

	// ワールドトランスフォーム
	worldTransform_.Initialize(model_->GetRootNode());

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// コマンドを受け取るか
	receiveCommand_ = true;

	// ステート
	StateInitialize();

	// パーツ
	PartInitialize();

	// コライダー
	ColliderInitialize();

	animation_.StartAnimation(kTutorialEnemyMotionStand, true);

}

void TutorialEnemy::Update()
{

	// ステート
	StateUpdate();

	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	localMatrixManager_->Map();

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

}

void TutorialEnemy::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void TutorialEnemy::ImGuiDraw()
{
}

void TutorialEnemy::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<PlayerAttack*>(colliderPartner)) {
		OnCollisionPlayerAttack(colliderPartner, collisionData);
	}

}

void TutorialEnemy::StateInitialize()
{

}

void TutorialEnemy::StateUpdate()
{

}

void TutorialEnemy::PartInitialize()
{

}

void TutorialEnemy::ColliderInitialize()
{

	// コライダー
	collider_ = std::make_unique<Capsule>();
	collider_->SetCollisionAttribute(collisionAttribute_);
	collider_->SetCollisionMask(collisionMask_);

	Segment segment = {
		worldTransform_.GetWorldPosition(),
		{ 0.0f, height_,0.0f }
	};

	collider_->Initialize(segment, 1.0f, this);

}

void TutorialEnemy::ColliderUpdate()
{

	collider_->segment_.origin_ = worldTransform_.GetWorldPosition();
	collider_->segment_.diff_ = { 0.0f, height_,0.0f };

}

void TutorialEnemy::AnimationUpdate()
{

}

void TutorialEnemy::OnCollisionPlayerAttack(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	animation_.StartAnimation(kTutorialEnemyMotionDamage, false);

}
