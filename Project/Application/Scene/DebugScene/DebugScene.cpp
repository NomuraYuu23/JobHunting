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

	ClothGPU::StaticInitialize(
		dxCommon_->GetDevice(),
		directionalLight_.get(),
		pointLightManager_.get(),
		spotLightManager_.get(),
		FogManager::GetInstance());

	clothScale_ = { 2.0f, 2.0f };
	clothDiv_ = { 15.0f, 15.0f };
	clothGPU_ = std::make_unique<ClothGPU>();
	clothGPU_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), clothScale_, clothDiv_, "Resources/default/clothDemo.png");
	clothGPU_->CollisionDataRegistration("plane", ClothGPUCollision::kCollisionTypeIndexPlane);
	for (uint32_t y = 0; y <= static_cast<uint32_t>(clothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
			clothGPU_->SetWeight(y, x, true);
			clothGPU_->SetPosition(y, x, {0.0f,3.0f,0.0f});
		}
	}

	// 左上
	clothGPU_->SetWeight(0, 0, false);
	clothGPU_->SetPosition(0, 0, { -clothScale_.x / 2.0f, 3.0f, -clothScale_.y / 2.0f });

	// 右上
	clothGPU_->SetWeight(0, static_cast<uint32_t>(clothDiv_.x), false);
	clothGPU_->SetPosition(0, static_cast<uint32_t>(clothDiv_.x), { clothScale_.x / 2.0f, 3.0f, -clothScale_.y / 2.0f });

	// 左下
	clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), 0, false);
	clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), 0, { -clothScale_.x / 2.0f, 3.0f, clothScale_.y / 2.0f });

	// 右下
	clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x), false);
	clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x), { clothScale_.x / 2.0f, 3.0f, clothScale_.y / 2.0f });

	IScene::InitilaizeCheck();

}

void DebugScene::Update()
{

	clothGPU_->Update(dxCommon_->GetCommadList());
	if (input_->TriggerKey(DIK_P)) {
		clothGPU_->SetWeight(0, 0, true);
		clothGPU_->SetWeight(0, static_cast<uint32_t>(clothDiv_.x), true);
		clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), 0, true);
		clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x), true);
	}
	if (input_->TriggerKey(DIK_R)) {
		for (uint32_t y = 0; y <= static_cast<uint32_t>(clothDiv_.y); ++y) {
			for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
				clothGPU_->SetWeight(y, x, true);
				clothGPU_->SetPosition(y, x, { 0.0f,3.0f,0.0f });
			}
		}

		// 左上
		clothGPU_->SetWeight(0, 0, false);
		clothGPU_->SetPosition(0, 0, { -clothScale_.x / 2.0f, 3.0f, -clothScale_.y / 2.0f });

		// 右上
		clothGPU_->SetWeight(0, static_cast<uint32_t>(clothDiv_.x), false);
		clothGPU_->SetPosition(0, static_cast<uint32_t>(clothDiv_.x), { clothScale_.x / 2.0f, 3.0f, -clothScale_.y / 2.0f });

		// 左下
		clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), 0, false);
		clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), 0, { -clothScale_.x / 2.0f, 3.0f, clothScale_.y / 2.0f });

		// 右下
		clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x), false);
		clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x), { clothScale_.x / 2.0f, 3.0f, clothScale_.y / 2.0f });

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


	//drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

	clothGPU_->Draw(dxCommon_->GetCommadList(), &camera_);

#pragma endregion

}

void DebugScene::ImguiDraw()
{

	clothGPU_->ImGuiDraw("clothGPU");

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
