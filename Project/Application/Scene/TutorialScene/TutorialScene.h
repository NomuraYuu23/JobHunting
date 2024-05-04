#pragma once

#include <array>

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../UI/UIManager.h"
#include "../../AudioManager/GameAudioManager.h"
#include "../../Skydome/Skydome.h"
#include "../../Ground/Ground.h"

#include "../../Object/Sample/SampleObject.h" // サンプルオブジェクト

#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/Collision2D/Collision2DDebugDraw.h"

// プレイヤー
#include "../../Player/Player.h"
#include "../../Camera/FollowCamera.h"

// エネミー
#include "../../TutorialEnemy/TutorialEnemy.h"


#include "../../UI/TutorialUIManager.h"


class TutorialScene : public IScene
{

public:

	~TutorialScene();

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

	/// <summary>
	/// 音量
	/// </summary>
	void LowerVolumeBGM();

private:

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	// ライト
	std::unique_ptr<DirectionalLight> directionalLight_;
	Vector3 direction = { 1.0f, -1.0f, 0.0f };
	float intencity = 1.0f;

	std::unique_ptr<CollisionManager> collisionManager_;

	// UIマネージャー
	std::unique_ptr<TutorialUIManager> uiManager_;
	std::array<uint32_t, TutorialUIManager::UIIndex::kUIIndexOfCount> uiTextureHandles_;

	// オーディオマネージャー
	std::unique_ptr<GameAudioManager> audioManager_;
	bool isDecreasingVolume = true;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// 点光源
	std::unique_ptr<PointLightManager> pointLightManager_;
	std::array<PointLightData, PointLightManager::kNumInstanceMax_> pointLightDatas_;

	// スポットライト
	std::unique_ptr<SpotLightManager> spotLightManager_;
	std::array<SpotLightData, SpotLightManager::kNumInstanceMax_> spotLightDatas_;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> playerWeaponModel_;

	// ボス
	std::unique_ptr<TutorialEnemy> enemy_;
	std::unique_ptr<Model> enemyModel_;

	// 追加カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// 地面
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Model> groundModel_;

};
