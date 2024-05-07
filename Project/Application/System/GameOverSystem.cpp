#include "GameOverSystem.h"

void GameOverSystem::Initialize()
{

	// 入力
	input_ = Input::GetInstance();

	// 動いているか
	isOperation_ = false;

	// リセット
	isReset_ = false;

}

void GameOverSystem::Update()
{

	//ゲームオーバーの文字が出てきている

	//ボタン待ち
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// リセットし始める
		isReset_ = true;
	}

}
