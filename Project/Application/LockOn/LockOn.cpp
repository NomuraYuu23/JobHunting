#include "LockOn.h"
#include "../Object/Character/Player/Player.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/2D/ImguiManager.h"

void LockOn::Initialize(uint32_t textureHandle)
{
}

void LockOn::Update(const std::list<BaseEnemy*>& enemies, Player* player, const BaseCamera& camera)
{
}

void LockOn::Draw()
{


}

void LockOn::ImGuiDraw()
{

	ImGui::Begin("LockOnMode");
	if (isAutomatic_) {
		ImGui::Text("Automatic");
	}
	else {
		ImGui::Text("Manual");
	}
	ImGui::End();

}

Vector3 LockOn::GetTargetPosition() const
{

	if (target_) {
		return target_->GetWorldTransformAdress()->GetWorldPosition();
	}

	return Vector3();

}

void LockOn::Restart()
{

	// ロックオンを外す
	target_ = nullptr;
	isAutomatic_ = false;

}

bool LockOn::OutOfRangeJudgment(Player* player, BaseCamera* camera)
{

	// 敵ロックオン座標取得
	Vector3 enemyWorldPosition = target_->GetWorldTransformAdress()->GetWorldPosition();

	//プレイヤー
	Vector3 playerWorldPosition = player->GetWorldTransformAdress()->GetWorldPosition();

	Vector3 dirEnemy = Vector3::Normalize(Vector3::Subtract(enemyWorldPosition, camera->GetTranslate()));
	Vector3 dirPlayer = Vector3::Normalize(Vector3::Subtract(playerWorldPosition, camera->GetTranslate()));
	float dot = Vector3::Dot(dirEnemy, dirPlayer);

	float distance = Vector3::Length(Vector3::Subtract(enemyWorldPosition, playerWorldPosition));
	// 距離条件チェック
	if (minDistance_ <= distance && distance <= maxDistance_ && dot > 0.0f) {
		return false;
	}

	return true;

}

void LockOn::LockOnModeChange()
{

	if (isAutomatic_) {
		isAutomatic_ = false;
	}
	else {
		isAutomatic_ = true;
	}

	target_ = nullptr;

}
