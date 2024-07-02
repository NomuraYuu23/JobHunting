#include "GhostStateChase.h"
#include "Ghost.h"
#include "../../../Engine/Math/Ease.h"

#include "../../Player/Player.h"

void GhostStateChase::Initialize()
{

	ghostMotionNo_ = kGhostMotionMove;

}

void GhostStateChase::Update()
{

	// ワールドトランスフォーム
	WorldTransform* worldTransform = ghost_->GetWorldTransformAdress();
	// 向きを使う
	worldTransform->usedDirection_ = true;

	// プレイヤー取得
	Player* player = ghost_->GetPlayer();

	// プレイヤーのワールドトランスフォーム
	WorldTransform* playerWorldTransform = player->GetWorldTransformAdress();

	// 現在の向き
	Vector3 direction = worldTransform->direction_;
	direction = Vector3::Normalize(Vector3::Subtract(playerWorldTransform->GetWorldPosition(), worldTransform->GetWorldPosition()));

	// 速度
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };

	// 速さ
	float speed = 0.01f;

	// 移動量に速さを反映
	move = Vector3::Multiply(speed, Vector3::Normalize(move));

	Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, direction);

	// 移動ベクトルをカメラの角度だけ回転する
	move = Matrix4x4::TransformNormal(move, rotateMatrix);

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	Vector3 targetDirection = { 0.0f,0.0f,0.0f };
	targetDirection.x = Vector3::Normalize(move).x;
	targetDirection.z = Vector3::Normalize(move).z;

	float targetAngleT = 0.02f;

	// 角度補間
	worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection, targetAngleT);

	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

}
