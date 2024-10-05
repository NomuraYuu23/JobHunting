#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include "UI.h"

class TutorialUIManager
{

public: // サブクラス

	enum UIIndex {
		kUIIndexAttack, // 攻撃
		kUIIndexRolling,// 回避
		kUIIndexHP,// HP
		kUIIndexTutorial, // チュートリアル
		kUIIndexGoGame, // ゲームへ
		kUIIndexOfCount,
	};

public: // 関数

	// 初期化
	void Initialize(const std::array<uint32_t, kUIIndexOfCount>& textureHandles);

	// 更新
	void Update();

	// 描画
	void Draw();

private: // 変数

	// 操作UI
	std::unique_ptr<Sprite> attackSprite_;
	std::unique_ptr<Sprite> rollingSprite_;

	// HP
	std::unique_ptr<Sprite> playerHPSprite_;
	std::unique_ptr<Sprite> playerHPFrameSprite_;
	float playerHPSizeX_;

	// チュートリアル
	std::unique_ptr<Sprite> tutorialSprite_;
	// ゲームへ
	std::unique_ptr<Sprite> goGameSprite_;

	std::array<uint32_t, kUIIndexOfCount> textureHandles_;

};

