#include "FlagPole2.h"
#include <random>

void FlagPole2::Initialize(LevelData::MeshData* data)
{

	BaseObstacle::Initialize(data);

	// 旗の上部分
	leftPart_ = { -1.0f,9.0f, 0.0f };
	leftPart_ = Matrix4x4::Transform(leftPart_, worldTransform_.parentMatrix_);

	// 旗の下部分
	rightPart_ = { 1.0f,9.0f, 0.0f };
	rightPart_ = Matrix4x4::Transform(rightPart_, worldTransform_.parentMatrix_);

	dxCommon_ = DirectXCommon::GetInstance();

	div_ = Vector2{ 15.0f, 15.0f };

	scale_ = Vector2{ 2.0f, 6.0f };

	cloth_ = std::make_unique<ClothGPU>();
	cloth_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), scale_, div_, "Resources/default/clothDemo.png");

	isPosSet_ = true;

}

void FlagPole2::Update()
{

	BaseObstacle::Update();

	cloth_->Update(dxCommon_->GetCommadList());

	if (isPosSet_) {
		FlagPosition();
		isPosSet_ = false;
	}

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distributionX(0.0f, 10.0f);
	std::uniform_real_distribution<float> distributionZ(-20.0f, 0.0f);

	Vector3 wind = { distributionX(randomEngine), 0.0f, distributionZ(randomEngine) };

	cloth_->SetWind(wind);

}

void FlagPole2::Draw(BaseCamera& camera)
{

	BaseObstacle::Draw(camera);

	cloth_->Draw(dxCommon_->GetCommadList(), &camera);

}

void FlagPole2::FlagPosition()
{

	// 
	for (uint32_t y = 0; y <= static_cast<uint32_t>(div_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(div_.x); ++x) {
			cloth_->SetWeight(y, x, true);
			cloth_->SetPosition(y, x, rightPart_);
		}
	}

	// 固定する
	cloth_->SetWeight(0, 0, false);
	cloth_->SetPosition(0, 0, leftPart_);
	cloth_->SetWeight(0, 1, false);
	cloth_->SetPosition(0, 1, leftPart_);
	cloth_->SetWeight(0, 2, false);
	cloth_->SetPosition(0, 2, leftPart_);

	cloth_->SetWeight(0, static_cast<uint32_t>(div_.x), false);
	cloth_->SetPosition(0, static_cast<uint32_t>(div_.x), rightPart_);
	cloth_->SetWeight(0, static_cast<uint32_t>(div_.x) - 1, false);
	cloth_->SetPosition(0, static_cast<uint32_t>(div_.x) - 1, rightPart_);
	cloth_->SetWeight(0, static_cast<uint32_t>(div_.x) - 2, false);
	cloth_->SetPosition(0, static_cast<uint32_t>(div_.x) - 2, rightPart_);

}
