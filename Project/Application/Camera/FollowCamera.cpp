#include "FollowCamera.h"
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"
#include "../../../Engine/Input/input.h"
#include <algorithm>
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Input/Input.h"
#include "../LockOn/LockOn.h"


void FollowCamera::Initialize() {

	BaseCamera::Initialize();

	BaseCamera::Update();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "moveRate", moveRate_);
	globalVariables->AddItem(groupName, "rotateRate", rotateRate_);
	globalVariables->AddItem(groupName, "offsetLength", offsetLength_);

	ApplyGlobalVariables();

}

void FollowCamera::Update(float elapsedTime) {

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	if (lockOn_) {
		if (lockOn_->ExistTarget()) {
			LockOnUpdate();
		}
		else {
			NotLockOnUpdate();
		}
	}
	else {
		NotLockOnUpdate();
	}

	//追従対象がいれば
	if (target_) {
		// 追従座標の補間
		Vector3 targetPos = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
		interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, targetPos, moveRate_);

		// オフセット
		Vector3 offset = OffsetCalc();

		transform_.translate = Vector3::Add(interTarget_, offset);

	}

	//ビュー更新
	BaseCamera::Update();

}

void FollowCamera::SetTarget(const WorldTransform* target)
{

	target_ = target;

	// 追従座標の補間
	Vector3 targetPos = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
	interTarget_ = targetPos;

	// オフセット
	Vector3 offset = OffsetCalc();

	transform_.translate = Vector3::Add(interTarget_, offset);

	//ビュー更新
	BaseCamera::Update();

}


Vector3 FollowCamera::OffsetCalc() const
{

	//追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, 3.0f, offsetLength_ };

	Matrix4x4 rotateMatrix;

	//カメラの角度から回転行列を計算する
	rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);


	//オフセットをカメラの回転に合わせて回転させる
	offset = Matrix4x4::TransformNormal(offset, rotateMatrix);

	return offset;

}

void FollowCamera::LockOnUpdate()
{

	// カメラをロックオン対象の方に向ける処理

	// ロックオン座標
	Vector3 lockOnPosition = lockOn_->GetTargetPosition();
	// ターゲット座標
	Vector3 targetPosition = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
	// 追従対象からロックオン対象へのベクトル(正規化)
	Vector3 sub = Vector3::Normalize(Vector3::Subtract(lockOnPosition, targetPosition));

	lockOnRotateMatrix_ = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, Vector3{ sub.x, 0.0f, sub.z });
	destinationAngle_.y = 0.0f;
	destinationAngle_.x = 0.0f;

}

void FollowCamera::NotLockOnUpdate()
{

	//インスタンス
	Input* input = Input::GetInstance();

	// スティック入力で角度を変更処理
	// 回転速度
	const float RotateSpeed = 0.000003f;

	destinationAngle_.y += input->GetRightAnalogstick().x * RotateSpeed;
	destinationAngle_.x += input->GetRightAnalogstick().y * RotateSpeed;

	// xに制限
	float limit = 3.14f / 4.0f;
	destinationAngle_.x = std::clamp(destinationAngle_.x, 0.0f, limit);

	transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngle_.y, rotateRate_);
	transform_.rotate.x = Math::LerpShortAngle(transform_.rotate.x, destinationAngle_.x, rotateRate_);

}

void FollowCamera::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";

	moveRate_ = globalVariables->GetFloatValue(groupName, "moveRate");
	rotateRate_ = globalVariables->GetFloatValue(groupName, "rotateRate");
	offsetLength_ = globalVariables->GetFloatValue(groupName, "offsetLength");

}
