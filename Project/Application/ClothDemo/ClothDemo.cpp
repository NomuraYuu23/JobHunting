#include "ClothDemo.h"
#include "../../Engine/2D/ImguiManager.h"

void ClothDemo::Initilalize(
	DirectionalLight* directionalLight,
	PointLightManager* pointLightManager,
	SpotLightManager* spotLightManager)
{

	dxCommon_ = DirectXCommon::GetInstance();

	input_ = Input::GetInstance();

	// 布の静的初期化
	ClothGPU::StaticInitialize(
		dxCommon_->GetDevice(),
		directionalLight,
		pointLightManager,
		spotLightManager,
		FogManager::GetInstance());

	clothFixedFunctions_[kFixedIndexEnd] = std::bind(&ClothDemo::ClothFixedEnd, this);
	clothFixedFunctions_[kFixedIndexTop] = std::bind(&ClothDemo::ClothFixedTop, this);

	// 布の大きさ
	clothScale_ = { 2.0f, 2.0f };
	// 布の分割数
	clothDiv_ = { 15.0f, 15.0f };
	// リセット位置
	resetPosition_ = { 0.0f,3.0f,0.0f };

	// 布の初期化
	clothGPU_ = std::make_unique<ClothGPU>();
	clothGPU_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), clothScale_, clothDiv_, "Resources/default/clothDemo.png");
	
	// 衝突オブジェクト
	clothGPU_->CollisionDataRegistration("plane", ClothGPUCollision::kCollisionTypeIndexPlane);
	//clothGPU_->CollisionDataRegistration("sphere", ClothGPUCollision::kCollisionTypeIndexSphere);

	ClothReset(kFixedIndexEnd);

}

void ClothDemo::Update()
{
	// 布CPU側処理
	clothGPU_->Update(dxCommon_->GetCommadList());

	// 入力処理
	if (input_->TriggerKey(DIK_P)) {
		RemoveFixation();
	}

	if (input_->TriggerKey(DIK_R)) {
		ClothReset(kFixedIndexEnd);
	}

	if (input_->TriggerKey(DIK_T)) {
		ClothReset(kFixedIndexTop);
	}

}

void ClothDemo::Draw(BaseCamera* camera)
{

	clothGPU_->Draw(dxCommon_->GetCommadList(), camera);

}

void ClothDemo::ImGuiDraw()
{

	clothGPU_->ImGuiDraw("clothGPU");

	ImGui::Begin("ClothDemo");
	ImGui::DragFloat3("ResetPosition", &resetPosition_.x, 0.01f);
	ImGui::End();

}

void ClothDemo::ClothReset(FixedIndex fixedIndex)
{

	// 全ての質点の固定をはずす、リセット位置へ
	for (uint32_t y = 0; y <= static_cast<uint32_t>(clothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
			clothGPU_->SetWeight(y, x, true);
			clothGPU_->SetPosition(y, x, resetPosition_);
		}
	}

	clothFixedFunctions_[fixedIndex]();

}

void ClothDemo::RemoveFixation()
{

	// 全ての質点の固定をはずす
	for (uint32_t y = 0; y <= static_cast<uint32_t>(clothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
			clothGPU_->SetWeight(y, x, true);
		}
	}

}

void ClothDemo::ClothFixedEnd()
{

	// 左上
	clothGPU_->SetWeight(0, 0, false);
	clothGPU_->SetPosition(0, 0, 
		{ resetPosition_.x  - clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z - clothScale_.y / 2.0f });
	// 右上
	clothGPU_->SetWeight(0, static_cast<uint32_t>(clothDiv_.x), false);
	clothGPU_->SetPosition(0, static_cast<uint32_t>(clothDiv_.x), 
		{ resetPosition_.x + clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z - clothScale_.y / 2.0f });
	// 左下
	clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), 0, false);
	clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), 0, 
		{ resetPosition_.x - clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z + clothScale_.y / 2.0f });
	// 右下
	clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x), false);
	clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x),
		{ resetPosition_.x + clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z + clothScale_.y / 2.0f });

}

void ClothDemo::ClothFixedTop()
{

	for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
		clothGPU_->SetWeight(0, x, false);
		clothGPU_->SetPosition(0, x, 
			{ resetPosition_.x - (clothScale_.x / 2.0f) + (clothScale_.x * (static_cast<float>(x) / clothDiv_.x)),
			resetPosition_.y, resetPosition_.z});
	}

}
