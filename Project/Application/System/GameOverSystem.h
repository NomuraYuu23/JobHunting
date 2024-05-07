#pragma once
#include "../../Engine/Input/Input.h"
#include "../../Engine/2D/Sprite.h"
class GameOverSystem
{

public: // 関数

	void Initialize(
		uint32_t titleTextureHandle, 
		uint32_t buttonTextureHandle);

	void Update();

	void UIDraw();

public: // アクセッサ

	void SetIsOperation(bool isOperation) { isOperation_ = isOperation; }

	bool GetIsOperation() { return isOperation_; }

	void SetIsReset(bool isReset) { isOperation_ = isReset; }

	bool GetIsReset() { return isReset_; }

private: // 変数

	// 入力
	Input* input_;

	// 動いているか
	bool isOperation_;

	// リセット
	bool isReset_;

	// ゲームオーバーの文字
	std::unique_ptr<Sprite> gameOverSprite_;
	uint32_t gameOverTextureHandle_;

	// ボタン
	std::unique_ptr<Sprite> buttonSprite_;
	uint32_t buttonTextureHandle_;
	// 点滅用媒介変数
	float buttonAlphaT_;
	// 点滅用媒介変数速度
	float buttonAlphaTSpeed_;
	// 点滅用媒介変数は増えるか
	bool buttonItIncreaseAlphaT_;
	// 色
	Vector4 buttonColor_;

};

