#include "ClothDemo.h"
#include "../../Engine/2D/ImguiManager.h"

void ClothDemo::Initilalize(
	DirectionalLight* directionalLight,
	PointLightManager* pointLightManager,
	SpotLightManager* spotLightManager)
{

	// エンジン
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
	clothDiv_ = { 63.0f, 63.0f };
	// リセット位置
	resetPosition_ = { 0.0f,3.0f,0.0f };

	// 衝突オブジェクト
	// 平面
	plane_ = std::make_unique<ClothDemoPlane>();
	plane_->Initialize("plane");

	// 球
	sphere_ = std::make_unique<ClothDemoSphere>();
	sphere_->Initialize("sphere");

	// 布の初期化
	ClothReset(dxCommon_->GetCommadListLoad());
	
	// リセット
	ClothPositionReset(kFixedIndexTop);

}

void ClothDemo::Update()
{
	// 布CPU側処理
	clothGPU_->Update(dxCommon_->GetCommadList());

	// 平面
	plane_->Update();
	ClothGPUCollision::CollisionDataMap planeData = plane_->GetData();
	clothGPU_->CollisionDataUpdate(plane_->GetName(), planeData);

	// 球
	sphere_->Update();
	ClothGPUCollision::CollisionDataMap sphereData = sphere_->GetData();
	clothGPU_->CollisionDataUpdate(sphere_->GetName(), sphereData);

}

void ClothDemo::Draw(BaseCamera* camera)
{

	clothGPU_->Draw(dxCommon_->GetCommadList(), camera);

}

void ClothDemo::ImGuiDraw()
{

	clothGPU_->ImGuiDraw("clothGPU");

	ImGui::Begin("ClothDemo");
	// 布
	ImGui::Text("Cloth");
	ImGui::DragFloat2("ClothDiv", &clothDiv_.x, 1.0f, 4.0f, 256.0f);
	ImGui::DragFloat2("ClothScale", &clothScale_.x, 0.01f, 1.0f);

	if (ImGui::Button("ClothReset")) {
		// 布の初期化
		ClothReset(dxCommon_->GetCommadList());
	}
	ImGui::DragFloat3("ResetPosition", &resetPosition_.x, 0.01f);
	if (ImGui::Button("RemoveFixation")) {
		RemoveFixation();
	}
	if (ImGui::Button("Reset_FixedEnd")) {
		ClothPositionReset(kFixedIndexEnd);
	}
	if (ImGui::Button("Reset_FixedTop")) {
		ClothPositionReset(kFixedIndexTop);
	}
	// 平面
	plane_->ImGuiDraw();
	if (ImGui::Button("PlaneSwitching")) {
		PlaneSwitching();
	}
	// 球
	sphere_->ImGuiDraw();
	if (ImGui::Button("SphereSwitching")) {
		SphereSwitching();
	}
	ImGui::End();

}

void ClothDemo::CollisionObjectDraw(BaseCamera* camera)
{

	plane_->Draw(*camera);
	sphere_->Draw(*camera);

}

void ClothDemo::ClothPositionReset(FixedIndex fixedIndex)
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

void ClothDemo::ClothReset(ID3D12GraphicsCommandList* commandList)
{

	// 布の計算データ一時保存
	Vector3 gravity{}; // 重力
	Vector3 wind{}; // 風力
	float stiffness = 0.0f; // 剛性。バネ定数k
	float speedResistance = 0.0f; // 速度抵抗
	float structuralShrink = 0.0f; // 構成バネ伸び抵抗
	float structuralStretch = 0.0f; // 構成バネ縮み抵抗
	float shearShrink = 0.0f; // せん断バネ伸び抵抗
	float shearStretch = 0.0f; // せん断バネ縮み抵抗
	float bendingShrink = 0.0f; // 曲げバネ伸び抵抗
	float bendingStretch = 0.0f; // 曲げバネ縮み抵抗
	int32_t relaxation = 1;

	if (clothGPU_) {
		gravity = clothGPU_->GetGravity(); // 重力
		wind = clothGPU_->GetWind(); // 風力
		stiffness = clothGPU_->GetStiffness(); // 剛性。バネ定数k
		speedResistance = clothGPU_->GetSpeedResistance(); // 速度抵抗
		// 抵抗
		structuralShrink = clothGPU_->GetStructuralStretch();
		structuralStretch = clothGPU_->GetStructuralShrink();
		shearShrink = clothGPU_->GetShearStretch();
		shearStretch = clothGPU_->GetShearShrink();
		bendingShrink = clothGPU_->GetBendingStretch();
		bendingStretch = clothGPU_->GetBendingShrink();
		// 更新回数
		relaxation = clothGPU_->GetRelaxation();
	}
	else {
		gravity = {0.0f,-9.8f, 0.0f}; // 重力
		wind = { 0.0f, 0.0f, 0.0f }; // 風力
		stiffness = 100.0f; // 剛性。バネ定数k
		speedResistance = 0.2f; // 速度抵抗
		// 抵抗 (structural > shear >= bending)の大きさが酔良い
		structuralShrink = 100.0f;
		structuralStretch = 100.0f;
		shearShrink = 80.0f;
		shearStretch = 80.0f;
		bendingShrink = 60.0f;
		bendingStretch = 60.0f;
		// 更新回数
		relaxation = 6; // 最大数
	}

	// 布の初期化
	clothGPU_.reset(nullptr);
	clothGPU_ = std::make_unique<ClothGPU>();
	clothGPU_->Initialize(dxCommon_->GetDevice(), commandList, clothScale_, clothDiv_, "Resources/default/clothDemo.png");

	// 布の計算データを戻す
	clothGPU_->SetGravity(gravity); // 重力
	clothGPU_->SetWind(wind); // 風力
	clothGPU_->SetStiffness(stiffness); // 剛性。バネ定数k
	clothGPU_->SetSpeedResistance(speedResistance); // 速度抵抗
	// 抵抗
	clothGPU_->SetStructuralStretch(structuralShrink);
	clothGPU_->SetStructuralShrink(structuralStretch);
	clothGPU_->SetShearStretch(shearShrink);
	clothGPU_->SetShearShrink(shearStretch);
	clothGPU_->SetBendingStretch(bendingShrink);
	clothGPU_->SetBendingShrink(bendingStretch);
	// 更新回数
	clothGPU_->SetRelaxation(relaxation);

	// 登録
	clothGPU_->CollisionDataRegistration(plane_->GetName(), ClothGPUCollision::kCollisionTypeIndexPlane);
	plane_->SetExsit(true);
	// 登録
	clothGPU_->CollisionDataRegistration(sphere_->GetName(), ClothGPUCollision::kCollisionTypeIndexSphere);
	sphere_->SetExsit(true);



	clothGPU_->CollisionDataRegistration("capsule", ClothGPUCollision::kCollisionTypeIndexCapsule);

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

void ClothDemo::PlaneSwitching()
{

	if (plane_->GetExist()) {
		// 削除
		clothGPU_->CollisionDataDelete(plane_->GetName());
		plane_->SetExsit(false);
	}
	else {
		// 登録
		clothGPU_->CollisionDataRegistration(plane_->GetName(), ClothGPUCollision::kCollisionTypeIndexPlane);
		plane_->SetExsit(true);
	}

}

void ClothDemo::SphereSwitching()
{

	if (sphere_->GetExist()) {
		// 削除
		clothGPU_->CollisionDataDelete(sphere_->GetName());
		sphere_->SetExsit(false);
	}
	else {
		// 登録
		clothGPU_->CollisionDataRegistration(sphere_->GetName(), ClothGPUCollision::kCollisionTypeIndexSphere);
		sphere_->SetExsit(true);
	}

}
