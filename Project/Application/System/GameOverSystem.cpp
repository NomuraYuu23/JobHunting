#include "GameOverSystem.h"
#include "../../Engine/Math/Ease.h"

void GameOverSystem::Initialize(
	uint32_t titleTextureHandle,
	uint32_t buttonTextureHandle)
{

	// 入力
	input_ = Input::GetInstance();

	// 動いているか
	isOperation_ = false;

	// リセット
	isReset_ = false;

	gameOverTextureHandle_ = titleTextureHandle;

	gameOverSprite_.reset(Sprite::Create(gameOverTextureHandle_, { 640.0f, 240.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));

	buttonTextureHandle_ = buttonTextureHandle;

	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, { 640.0f, 550.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
	buttonAlphaT_ = 0.0f;
	buttonAlphaTSpeed_ = 0.01f;
	buttonItIncreaseAlphaT_ = true;
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

}

void GameOverSystem::Update()
{

	//ゲームオーバーの文字が出てきている


	// ボタン
	if (buttonItIncreaseAlphaT_) {
		buttonAlphaT_ += buttonAlphaTSpeed_;
		if (buttonAlphaT_ >= 1.0f) {
			buttonAlphaT_ = 1.0f;
			buttonItIncreaseAlphaT_ = false;
		}
	}
	else {
		buttonAlphaT_ -= buttonAlphaTSpeed_;
		if (buttonAlphaT_ <= 0.0f) {
			buttonAlphaT_ = 0.0f;
			buttonItIncreaseAlphaT_ = true;
		}
	}
	buttonColor_.w = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, buttonAlphaT_);
	buttonSprite_->SetColor(buttonColor_);

	// ボタンスプライト
	if (buttonItIncreaseAlphaT_) {
		buttonAlphaT_ += buttonAlphaTSpeed_;
		if (buttonAlphaT_ >= 1.0f) {
			buttonAlphaT_ = 1.0f;
			buttonItIncreaseAlphaT_ = false;
		}
	}
	else {
		buttonAlphaT_ -= buttonAlphaTSpeed_;
		if (buttonAlphaT_ <= 0.0f) {
			buttonAlphaT_ = 0.0f;
			buttonItIncreaseAlphaT_ = true;
		}
	}
	buttonColor_.w = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, buttonAlphaT_);
	buttonSprite_->SetColor(buttonColor_);

	//ボタン待ち
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// リセットし始める
		isReset_ = true;
	}

}

void GameOverSystem::UIDraw()
{

	gameOverSprite_->Draw();
	buttonSprite_->Draw();

}
