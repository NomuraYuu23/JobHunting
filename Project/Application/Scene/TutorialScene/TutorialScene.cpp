#include "TutorialScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/ModelDraw.h"

// 仮

#include "../../Player/PlayerState/PlayerStateAttack/PlayerStateAttack.h"

TutorialScene::~TutorialScene()
{

	if (stopAudio_) {
		for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
			audioManager_->StopWave(i);
		}
	}

	particleManager_->Finalize();

}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	// ビュープロジェクション
	EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	//パーティクル
	particleManager_ = ParticleManager::GetInstance();
	std::array<Model*, ParticleModelIndex::kCountofParticleModelIndex> particleModel;
	particleModel[ParticleModelIndex::kUvChecker] = particleUvcheckerModel_.get();
	particleModel[ParticleModelIndex::kCircle] = particleCircleModel_.get();
	particleManager_->ModelCreate(particleModel);


	isDebugCameraActive_ = false;

	collisionManager_.reset(new CollisionManager);
	collisionManager_->Initialize();

	// UI
	uiManager_ = std::make_unique<TutorialUIManager>();
	uiManager_->Initialize(uiTextureHandles_);

	// オーディオマネージャー
	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->Initialize();

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// 地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 点光源
	pointLightManager_ = std::make_unique<PointLightManager>();
	pointLightManager_->Initialize();
	for (size_t i = 0; i < pointLightDatas_.size(); ++i) {
		pointLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		pointLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		pointLightDatas_[i].intencity = 1.0f;
		pointLightDatas_[i].radius = 10.0f;
		pointLightDatas_[i].decay = 10.0f;
		pointLightDatas_[i].used = false;
	}

	spotLightManager_ = std::make_unique<SpotLightManager>();
	spotLightManager_->Initialize();
	for (size_t i = 0; i < spotLightDatas_.size(); ++i) {
		spotLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		spotLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		spotLightDatas_[i].intencity = 1.0f;
		spotLightDatas_[i].direction = { 0.0f, -1.0f, 0.0f }; // ライトの方向
		spotLightDatas_[i].distance = 10.0f; // ライトの届く距離
		spotLightDatas_[i].decay = 2.0f; // 減衰率
		spotLightDatas_[i].cosAngle = 2.0f; // スポットライトの余弦
		spotLightDatas_[i].cosFalloffStart = 1.0f; // フォールオフ開始位置
		spotLightDatas_[i].used = false; // 使用している
	}

	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get(), playerWeaponModel_.get());
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransformAdress());
	player_->SetCamera(static_cast<BaseCamera*>(followCamera_.get()));

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModel_.get());
	enemy_->SetPlayer(player_.get());

}

/// <summary>
/// 更新処理
/// </summary>
void TutorialScene::Update() {

#ifdef _DEBUG
	ImguiDraw();
#endif

	if (requestSceneNo == kClear || requestSceneNo == kTitle || isBeingReset_) {
		resetScene_ = false;
		// BGM音量下げる
		if (isDecreasingVolume) {
			LowerVolumeBGM();
		}
		return;
	}

	// ゲームへ
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonY)) {
		requestSceneNo = kGame;
	}

	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = Vector3::Normalize(direction);
	directionalLightData.intencity = intencity;
	directionalLight_->Update(directionalLightData);

	pointLightManager_->Update(pointLightDatas_);
	spotLightManager_->Update(spotLightDatas_);

	// 追従カメラ
	followCamera_->Update();

	camera_ = static_cast<BaseCamera>(*followCamera_.get());

	player_->Update();

	enemy_->Update();

	// あたり判定
	collisionManager_->ListClear();
	collisionManager_->ListRegister(player_->GetCollider());
	collisionManager_->ListRegister(enemy_->GetCollider());

	// プレイヤーの攻撃
	if (player_->GetCurrentStateNo() == kPlayerStateAttack) {
		if (static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetIsAttackJudgment()) {
			collisionManager_->ListRegister(static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetCollider());
		}
	}

	collisionManager_->CheakAllCollision();

	// スカイドーム
	skydome_->Update();

	// 地面
	ground_->Update();

	uiManager_->Update();

	// デバッグカメラ
	DebugCameraUpdate();

	//パーティクル
	particleManager_->Update(camera_);

}

/// <summary>
/// 描画処理
/// </summary>
void TutorialScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();


#pragma endregion

#pragma region モデル描画

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pipelineStateIndex = ModelDraw::kPipelineStateIndexAnimObject;
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();
	ModelDraw::PreDraw(preDrawDesc);

	//3Dオブジェクトはここ

	// スカイドーム
	skydome_->Draw(camera_);
	// 地面
	ground_->Draw(camera_);

	// プレイヤー
	player_->Draw(camera_);

	// エネミー
	enemy_->Draw(camera_);

#ifdef _DEBUG

	// デバッグ描画
	//colliderDebugDraw_->Draw(camera_);

#endif // _DEBUG

	ModelDraw::PostDraw();

#pragma endregion

#pragma region パーティクル描画

	preDrawDesc.pipelineStateIndex = ModelDraw::kPipelineStateIndexParticle;
	ModelDraw::PreDraw(preDrawDesc);

	// パーティクルはここ
	particleManager_->Draw(camera_.GetViewProjectionMatrix());

	ModelDraw::PostDraw();

#pragma endregion

#ifdef _DEBUG
#pragma region コライダー2d描画

#pragma endregion
#endif // DEBUG_


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画

	// UIマネージャー
	uiManager_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TutorialScene::ImguiDraw() {
#ifdef _DEBUG

	ImGui::Begin("Framerate");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	// スカイドーム
	skydome_->ImGuiDraw();

	debugCamera_->ImGuiDraw();

#endif // _DEBUG

}

void TutorialScene::DebugCameraUpdate()
{

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}
#endif

}

void TutorialScene::ModelCreate()
{
	// パーティクル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.gltf", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

	//プレイヤー
	playerModel_.reset(Model::Create("Resources/Model/Player/", "Player.gltf", dxCommon_, textureHandleManager_.get()));
	playerWeaponModel_.reset(Model::Create("Resources/Model/Player/", "PlayerWeapon.gltf", dxCommon_, textureHandleManager_.get()));

	// 地面
	groundModel_.reset(Model::Create("Resources/Model/Ground/", "Ground.obj", dxCommon_, textureHandleManager_.get()));

	// エネミー
	enemyModel_.reset(Model::Create("Resources/Model/Boss/", "Boss.gltf", dxCommon_, textureHandleManager_.get()));
}

void TutorialScene::TextureLoad()
{

	// UI
	uiTextureHandles_ = {
		TextureManager::Load("Resources/UI/attack.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/UI/rolling.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/default/white2x2.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/UI/tutorial.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/UI/goGame.png", dxCommon_, textureHandleManager_.get()),
	};

}

void TutorialScene::LowerVolumeBGM()
{


	const uint32_t startHandleIndex = 3;

	//for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
	//	if (audioManager_->GetPlayingSoundDatas()[i].handle_ == kGameAudioNameIndexBGM + startHandleIndex) {
	//		float decreasingVolume = 1.0f / 60.0f;
	//		float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
	//		if (volume < 0.0f) {
	//			volume = 0.0f;
	//			audioManager_->StopWave(i);
	//			isDecreasingVolume = false;
	//		}
	//		else {
	//			audioManager_->SetPlayingSoundDataVolume(i, volume);
	//			audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
	//		}
	//	}
	//}

}
