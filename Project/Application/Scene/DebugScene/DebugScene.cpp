#include "DebugScene.h"

DebugScene::~DebugScene()
{
}

void DebugScene::Initialize()
{

	IScene::Initialize();

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

	clothDemo_ = std::make_unique<ClothDemo>();
	clothDemo_->Initilalize(directionalLight_.get(), pointLightManager_.get(), spotLightManager_.get());

	isDebugCameraActive_ = true;
	// デバッグカメラ初期位置
	EulerTransform cameraTransform =
	{
		1.0f, 1.0f, 1.0f,
		0.45f, -0.75f, 0.0f,
		10.5f, 9.0f, -11.0f
	};
	debugCamera_->SetTransform(cameraTransform);

	IScene::InitilaizeCheck();

}

void DebugScene::Update()
{

	clothDemo_->Update();

	DebugCameraUpdate();

	ImguiDraw();

}

void DebugScene::Draw()
{

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;
	ModelDraw::PreDraw(preDrawDesc);

	// スカイドーム
	skydome_->Draw(camera_);

	clothDemo_->CollisionObjectDraw(&camera_);

	ModelDraw::PostDraw();

#pragma region 線描画

	clothDemo_->Draw(&camera_);

#pragma endregion

}

void DebugScene::ImguiDraw()
{

	clothDemo_->ImGuiDraw();

	debugCamera_->ImGuiDraw();

}

void DebugScene::DebugCameraUpdate()
{

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}

}
