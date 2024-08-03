#pragma once

#include <cstdint>

class AxSpearMan;

class AxSpearManCommand
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="axSpearMan">斧槍男</param>
	void Initialize(AxSpearMan* axSpearMan);

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private:

	// 斧槍男
	AxSpearMan* axSpearMan_;

public:

	// 斧槍男
	void SetGhost(AxSpearMan* axSpearMan) { axSpearMan_ = axSpearMan; }

};

