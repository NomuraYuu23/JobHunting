#include "LockOn.h"
#include "../Object/Character/Player/Player.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/2D/ImguiManager.h"

void LockOn::Initialize(uint32_t textureHandle)
{

	//スプライトの生成関数
	lockOnMark_.reset(
		Sprite::Create(textureHandle, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));

}

void LockOn::Update(const std::list<BaseEnemy*>& enemies, Player* player, BaseCamera* camera)
{

	// 入力確認
	Input* input = Input::GetInstance();

	// トリガー変数
	bool triggerJoystickButtonRST = input->TriggerJoystick(kJoystickButtonRST);
	bool triggerJoystickButtonLB = input->TriggerJoystick(kJoystickButtonLB);

	// ロックオン状態なら
	if (target_) {
		// ロックオン解除処理
		if (triggerJoystickButtonRST) {
			// ロックオンを外す
			target_ = nullptr;
		}
		// 範囲外判定
		else if (OutOfRangeJudgment(player, camera)) {
			// ロックオンを外す
			target_ = nullptr;
		}
		else if (triggerJoystickButtonLB) {
			// ロックオン対象の検索
			// ロックオン対象の絞り込み
			// 目標
			std::list<std::pair<float, BaseEnemy*>> targets;

			// 全ての敵に対して順にロックオン判定
			for (BaseEnemy* enemy : enemies) {
				// 敵ロックオン座標取得
				Vector3 enemyWorldPosition = enemy->GetWorldTransformAdress()->GetWorldPosition();

				//プレイヤー
				Vector3 playerWorldPosition = player->GetWorldTransformAdress()->GetWorldPosition();

				Vector3 dirEnemy = Vector3::Normalize(Vector3::Subtract(enemyWorldPosition, camera->GetTranslate()));
				Vector3 dirPlayer = Vector3::Normalize(Vector3::Subtract(playerWorldPosition, camera->GetTranslate()));
				float dot = Vector3::Dot(dirEnemy, dirPlayer);

				float distance = Vector3::Length(Vector3::Subtract(enemyWorldPosition, playerWorldPosition));
				// 距離条件チェック
				if (minDistance_ <= distance && distance <= maxDistance_ && dot > 0.0f && enemy != target_) {

					targets.emplace_back(std::make_pair(distance, enemy));

				}

				// ロックオン対象をリセット
				target_ = nullptr;
				if (targets.size() > 0) {
					// 距離で昇順にソート
					targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
					// ソートの結果一番近い敵をロックオン対象とする
					target_ = targets.front().second;
				}
			}
		}

	}
	else {
		// ロックオン対象の検索
		if (triggerJoystickButtonRST) {
			// ロックオン対象の検索
			// ロックオン対象の絞り込み
			// 目標
			std::list<std::pair<float, BaseEnemy*>> targets;

			// 全ての敵に対して順にロックオン判定
			for (BaseEnemy* enemy : enemies) {
				// 敵ロックオン座標取得
				Vector3 enemyWorldPosition = enemy->GetWorldTransformAdress()->GetWorldPosition();

				//プレイヤー
				Vector3 playerWorldPosition = player->GetWorldTransformAdress()->GetWorldPosition();

				Vector3 dirEnemy = Vector3::Normalize(Vector3::Subtract(enemyWorldPosition, camera->GetTranslate()));
				Vector3 dirPlayer = Vector3::Normalize(Vector3::Subtract(playerWorldPosition, camera->GetTranslate()));
				float dot = Vector3::Dot(dirEnemy, dirPlayer);

				float distance = Vector3::Length(Vector3::Subtract(enemyWorldPosition, playerWorldPosition));
				// 距離条件チェック
				if (minDistance_ <= distance && distance <= maxDistance_ && dot > 0.0f) {

					targets.emplace_back(std::make_pair(distance, enemy));

				}

				// ロックオン対象をリセット
				target_ = nullptr;
				if (targets.size() > 0) {
					// 距離で昇順にソート
					targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
					// ソートの結果一番近い敵をロックオン対象とする
					target_ = targets.front().second;
				}

			}

		}
	}

	// ロックオン状態なら
	if (target_) {
		// ロックオンマークの座標計算
		// 敵のロックオン座標取得
		Vector3 positionWorld = target_->GetWorldTransformAdress()->GetWorldPosition();
		positionWorld.y = target_->GetHeight();
		// ワールド座標からスクリーン座標に変換
		Matrix4x4 viewPort = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matViewProjectionViewPort = Matrix4x4::Multiply(camera->GetViewProjectionMatrix(), viewPort);
		Vector3 positionScreen = Matrix4x4::Transform(positionWorld, matViewProjectionViewPort);
		// Vector2に格納
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);
		// スプライトの座標を設定
		lockOnMark_->SetPosition(positionScreenV2);
	}

}

void LockOn::Draw()
{

	if (target_) {
		lockOnMark_->Draw();
	}

}

void LockOn::ImGuiDraw()
{

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

