#include "AxSpearManBeamAttack.h"

void AxSpearManBeamAttack::Initialize(WorldTransform* parent)
{

	// あたり判定ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

	// 攻撃のプレイヤーからのローカル位置
	center_ = { 0.0f,0.0f, 1.5f };

	// 攻撃の回転
	rotateMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 攻撃のサイズ
	size_ = { 1.0f,1.0f,1.0f };

	// あたり判定コライダー
	ColliderShape* collider = new ColliderShape();
	OBB obb;
	obb.Initialize(center_, rotateMatrix_, size_, this);
	obb.SetCollisionAttribute(0x00000002);
	obb.SetCollisionMask(0xfffffffd);
	*collider = obb;

	collider_.reset(collider);

	// あたり判定を取るか
	isAttackJudgment_ = false;

}

void AxSpearManBeamAttack::Update()
{

	worldTransform_.transform_.translate = center_;
	worldTransform_.UpdateMatrix();

	Vector3 center = worldTransform_.GetWorldPosition();

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = center;
	obb.SetOtientatuons(rotateMatrix_);
	obb.size_ = size_;

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}
