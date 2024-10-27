#pragma once
#include <array>

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../UI/UIManager.h"
#include "../../Skydome/Skydome.h"

#include "../../Object/Sample/SampleObject.h" // サンプルオブジェクト

#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/Collision2D/Collision2DDebugDraw.h"

#include "../../Camera/FollowCamera.h"
#include "../../LockOn/LockOn.h"

// ゲームオーバーシステム
#include "../../System/GameOverSystem.h"
#include "../../System/BossSystem.h"

class GameScene : public IScene
{

public:

	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	std::unique_ptr<CollisionManager> collisionManager_;

	// UIマネージャー
	std::unique_ptr<UIManager> uiManager_;
	std::array<uint32_t, UIIndex::kUIIndexOfCount> uiTextureHandles_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// 追加カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// ロックオン
	std::unique_ptr<LockOn> lockOn_;
	uint32_t lockOnTextureHandle_;

	// ゲームオーバーシステム
	std::unique_ptr<GameOverSystem> gameOverSystem_;
	uint32_t gameOverButtonTextureHandle_;
	uint32_t gameOverStringTextureHandle_;

	// スカイボックス
	uint32_t skyboxTextureHandle_ = 0;

	// プレイヤー
	Player* player_ = nullptr;

	// ボス
	BaseEnemy* bossEnemy_ = nullptr;

	// ボスシステム
	std::unique_ptr<BossSystem> bossSystem_;

	// エフェクトマネージャー
	EffectManager* effectManager_ = nullptr;

};
