#include "Enemy.h"
#include "../../Engine/Collider/Capsule/Capsule.h"
#include "../../Engine/2D/ImguiManager.h"
#include "../Player/Player.h"
#include <cassert>
#include "../../Engine/3D/ModelDraw.h"
#include "../../Engine/Animation/LocalMatrixDraw.h"

void Enemy::Initialize(Model* model)
{

	// モデル
	model_ = model;

	// マテリアル
	material_.reset(Material::Create());
	material_->SetEnableLighting(BlinnPhongReflection);

	// ワールドトランスフォーム
	worldTransform_.Initialize(model_->GetRootNode());
	worldTransform_.transform_.translate.z = 10.0f;
	worldTransform_.transform_.translate.x = -0.1f;

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// コマンド
	enemyCommand_ = EnemyCommand::GetInstance();
	enemyCommand_->Initialize();

	// コマンドを受け取るか
	receiveCommand_ = true;

	// ステート
	StateInitialize();

	// パーツ
	PartInitialize();

	// コライダー
	ColliderInitialize();

	// hp
	hp_ = 10;
	initHp_ = 10;

	isDead_ = false;
	
	enemyAttack_ = std::make_unique<EnemyAttack>();
	enemyAttack_->Initialize(&worldTransform_);

}

void Enemy::Update()
{

	if (receiveCommand_) {
		nextStateNo_ = enemyCommand_->Command();
	}

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

void Enemy::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void Enemy::ImGuiDraw()
{

}

void Enemy::DebugDrawMap(DrawLine* drawLine)
{

	LocalMatrixDraw::DrawMap(
		localMatrixManager_.get(),
		&worldTransform_,
		drawLine);

}

void Enemy::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<PlayerAttack*>(colliderPartner)) {
		OnCollisionPlayerAttack(colliderPartner, collisionData);
	}

}

void Enemy::StateInitialize()
{

	// ステートファクトリー
	enemyStateFactory_ = EnemyStateFactory::GetInstance();

	// ステート
	enemyState_.reset(enemyStateFactory_->CreateEnemyState(kEnemyStateRoot)); // 最初のステート
	enemyState_->Initialize();

	// ステート番号
	currentStateNo_ = EnemyState::kEnemyStateRoot; // 最初のステート
	prevStateNo_ = EnemyState::kEnemyStateRoot; // 最初のステート
	nextStateNo_ = EnemyState::kEnemyStateRoot; // 最初のステート
	enemyState_->SetEnemy(this); // エネミーセット

}

void Enemy::StateUpdate()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (receiveCommand_) {
		currentStateNo_ = nextStateNo_;
	}
	else {
		currentStateNo_ = enemyState_->GetEnemyStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		enemyState_.reset(enemyStateFactory_->CreateEnemyState(currentStateNo_));
		enemyState_->Initialize();
	}

	// ステート更新
	enemyState_->Update();

}

void Enemy::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = EnemyMotionIndex::kEnemyMotionStand;

	// 前のモーション番号
	prevMotionNo_ = EnemyMotionIndex::kEnemyMotionStand;

	// 待ちアニメーション
	animation_.StartAnimation(kEnemyMotionStand, true);

}

void Enemy::ColliderInitialize()
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

void Enemy::ColliderUpdate()
{

	collider_->segment_.origin_ = worldTransform_.GetWorldPosition();
	collider_->segment_.diff_ = { 0.0f, height_,0.0f };

}

void Enemy::AnimationUpdate()
{

	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = enemyState_->GetEnemyMotionNo();

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

}

void Enemy::Damage(uint32_t damage)
{

	hp_ -= damage;

	if (hp_ <= 0) {
		isDead_ = true;
	}

}

float Enemy::RatioHP()
{

	assert(initHp_ != 0);

	return static_cast<float>(hp_) / static_cast<float>(initHp_);
}

void Enemy::OnCollisionPlayerAttack(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

}
