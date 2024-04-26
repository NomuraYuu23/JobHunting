#include "TutorialUIManager.h"

void TutorialUIManager::Initialize(const std::array<uint32_t, kUIIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	Vector2 position = { 640.0f, 360.0f };
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 操作
	position = { 1280.0f - 160.0f - 20.0f, 720.0f - 40.0f - 20.0f - 80.0f };
	attackSprite_.reset(Sprite::Create(textureHandles_[kUIIndexAttack], position, color));
	position = { 1280.0f - 160.0f - 20.0f, 720.0f - 40.0f - 20.0f };
	rollingSprite_.reset(Sprite::Create(textureHandles_[kUIIndexRolling], position, color));

	// HPプレイヤー
	position = { 25.0f, 25.0f };
	color = { 0.8f, 0.0f, 0.0f, 0.8f };
	playerHPSprite_.reset(Sprite::Create(textureHandles_[kUIIndexHP], position, color));
	playerHPSprite_->SetAnchorPoint(Vector2{ 0.0f, 0.0f });
	playerHPSprite_->SetSize(Vector2{ 200.0f, 20.0f });
	playerHPSizeX_ = playerHPSprite_->GetSize().x;

	position = { 20.0f, 20.0f };
	color = { 0.1f, 0.1f, 0.1f, 1.0f };
	playerHPFrameSprite_.reset(Sprite::Create(textureHandles_[kUIIndexHP], position, color));
	playerHPFrameSprite_->SetAnchorPoint(Vector2{ 0.0f, 0.0f });
	playerHPFrameSprite_->SetSize(Vector2{ 210.0f, 30.0f });

	position = { 880.0f, 20.0f };
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
	tutorialSprite_.reset(Sprite::Create(textureHandles_[kUIIndexTutorial], position, color));


	position = { 880.0f, 100.0f };
	goGameSprite_.reset(Sprite::Create(textureHandles_[kUIIndexGoGame], position, color));

}

void TutorialUIManager::Update()
{
}

void TutorialUIManager::Draw()
{

	// 操作
	attackSprite_->Draw();
	rollingSprite_->Draw();

	// プレイヤーHP
	playerHPFrameSprite_->Draw();
	playerHPSprite_->Draw();

	// チュートリアル
	tutorialSprite_->Draw();
	// ゲームへ
	goGameSprite_->Draw();

}
