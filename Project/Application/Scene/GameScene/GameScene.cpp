#include "GameScene.h"
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
#include "../../../Engine/base/WindowSprite.h"

GameScene::~GameScene()
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
void GameScene::Initialize() {

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
	uiManager_ = std::make_unique<UIManager>();
	uiManager_->Initialize(uiTextureHandles_);

	// オーディオマネージャー
	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->Initialize();

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

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
	//player_->Initialize(playerModel_.get(), playerWeaponModel_.get());
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransformAdress());
	player_->SetCamera(static_cast<BaseCamera*>(followCamera_.get()));

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModel_.get());
	enemy_->SetPlayer(player_.get());

	gameOverSystem_ = std::make_unique<GameOverSystem>();
	gameOverSystem_->Initialize(gameOverStringTextureHandle_, gameOverButtonTextureHandle_);

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

#ifdef _DEBUG
	ImguiDraw();
#endif

	if (requestSceneNo_ == kClear || requestSceneNo_ == kTitle || isBeingReset_) {
		resetScene_ = false;
		// BGM音量下げる
		if (isDecreasingVolume) {
			LowerVolumeBGM();
		}
		return;
	}

	// タイトルへ
	if (enemy_->GetIsDead()) {
		requestSceneNo_ = kClear;
	}
	if (player_->GetIsDead()) {
		gameOverSystem_->SetIsOperation(true);
	}

	if (gameOverSystem_->GetIsOperation()) {
		gameOverSystem_->Update();
		if (gameOverSystem_->GetIsReset()) {
			resetScene_ = true;
			isBeingReset_ = true;
			gameOverSystem_->SetIsReset(false);
		}
		return;
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
	//collisionManager_->ListRegister(player_->GetCollider());
	//collisionManager_->ListRegister(enemy_->GetCollider());

	// プレイヤーの攻撃
	if (player_->GetPlayerAttack()->GetIsAttackJudgment()) {
		//collisionManager_->ListRegister(player_->GetPlayerAttack()->GetCollider());
	}

	// エネミーの攻撃
	if (enemy_->GetEnemyAttack()->GetIsAttackJudgment()) {
		//collisionManager_->ListRegister(enemy_->GetEnemyAttack()->GetCollider());
	}

	collisionManager_->CheakAllCollision();

	// スカイドーム
	skydome_->Update();

	// 地面
	ground_->Update();

	uiManager_->Update(player_->RatioHP(), enemy_->RatioHP());

	// デバッグカメラ
	DebugCameraUpdate();

	//パーティクル
	particleManager_->Update(camera_);

#ifdef _DEBUG

	player_->DebugDrawMap(drawLine_);
	enemy_->DebugDrawMap(drawLine_);

#endif // _DEBUG

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

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
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;
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

#pragma region 線描画

	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

#pragma endregion

#pragma region パーティクル描画

	// パーティクルはここ
	particleManager_->Draw(camera_.GetViewProjectionMatrix(), dxCommon_->GetCommadList());

#pragma endregion

	PostEffect::GetInstance()->SetKernelSize(33);
	PostEffect::GetInstance()->SetThreshold(0.0f);

	PostEffect::GetInstance()->Execution(
		dxCommon_->GetCommadList(),
		renderTargetTexture_,
		PostEffect::kCommandIndexBloom
	);

	renderTargetTexture_->ClearDepthBuffer();

	WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());

	if (gameOverSystem_->GetIsOperation() || isBeingReset_) {

		PostEffect::GetInstance()->SetColorPosition(Vector2{0.5f,0.5f});
		PostEffect::GetInstance()->SetColorSize(Vector2{ 10.0f,10.0f });
		PostEffect::GetInstance()->SetColorLerpT(1.0f);

		PostEffect::GetInstance()->Execution(
			dxCommon_->GetCommadList(),
			renderTargetTexture_,
			PostEffect::kCommandIndexGrayScale
		);

		renderTargetTexture_->ClearDepthBuffer();

		WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());

	}
	else if (player_->GetHp() == 1) {

		PostEffect::GetInstance()->Execution(
			dxCommon_->GetCommadList(),
			renderTargetTexture_,
			PostEffect::kCommandIndexVignette
		);

		renderTargetTexture_->ClearDepthBuffer();

		WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());

	}

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

	if (gameOverSystem_->GetIsOperation() || isBeingReset_) {
		gameOverSystem_->UIDraw();
	}
	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw(){
#ifdef _DEBUG

	ImGui::Begin("Framerate");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	// スカイドーム
	skydome_->ImGuiDraw();

	debugCamera_->ImGuiDraw();

#endif // _DEBUG

}

void GameScene::DebugCameraUpdate()
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

void GameScene::ModelCreate()
{

	// パーティクル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.gltf", dxCommon_));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_));
	
	//プレイヤー
	playerModel_.reset(Model::Create("Resources/Model/Player/", "Player.gltf", dxCommon_ ));
	playerWeaponModel_.reset(Model::Create("Resources/Model/Player/", "PlayerWeapon.gltf", dxCommon_));

	// 地面
	groundModel_.reset(Model::Create("Resources/Model/Ground/", "Ground.obj", dxCommon_));

	// エネミー
	enemyModel_.reset(Model::Create("Resources/Model/Boss/", "Boss.gltf", dxCommon_));

}

void GameScene::TextureLoad()
{

	// UI
	uiTextureHandles_ = {
		TextureManager::Load("Resources/UI/attack.png", dxCommon_),
		TextureManager::Load("Resources/UI/rolling.png", dxCommon_),
		TextureManager::Load("Resources/default/white2x2.png", dxCommon_),
	};

	// ゲームオーバー
	gameOverButtonTextureHandle_ = TextureManager::Load("Resources/OutGame/button.png", dxCommon_);
	gameOverStringTextureHandle_ = TextureManager::Load("Resources/UI/gameOver.png", dxCommon_);

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

}

void GameScene::LowerVolumeBGM()
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
