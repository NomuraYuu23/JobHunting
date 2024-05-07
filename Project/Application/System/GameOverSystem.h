#pragma once
#include "../../Engine/Input/Input.h"
class GameOverSystem
{

public: // 関数

	void Initialize();

	void Update();

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

};

