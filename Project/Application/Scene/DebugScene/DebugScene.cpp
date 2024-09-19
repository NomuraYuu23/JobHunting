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

	cloth_ = std::make_unique<Cloth>();
	cloth_->Initialize(Vector2{ 2.0f, 2.0f }, Vector2{15.0f, 15.0f});
	ClothModel::StaticInitialize(directionalLight_.get(),pointLightManager_.get(), spotLightManager_.get(), FogManager::GetInstance());

	IScene::InitilaizeCheck();

}

void DebugScene::Update()
{

	cloth_->Update();

	if (input_->TriggerKey(DIK_P)) {
		cloth_->SetPosition(10, 10, Vector3{ 1.0f,1.0f,-1.0f });
	}

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

	ModelDraw::PostDraw();

#pragma region 線描画


	cloth_->Draw(dxCommon_->GetCommadList(), &camera_);
	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

#pragma endregion

}

void DebugScene::ImguiDraw()
{

	cloth_->ImGuiDraw();

	debugCamera_->ImGuiDraw();

}

void DebugScene::DebugCameraUpdate()
{

	if (input_->TriggerKey(DIK_RSHIFT)) {
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

}
