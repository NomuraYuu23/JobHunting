#include "FlagPole.h"
#include <random>

void FlagPole::Initialize(LevelData::MeshData* data)
{

	BaseObstacle::Initialize(data);

	// 旗の上部分
	upperPart_ = {0.0f,9.0f,0.0f};

	upperPart_ = Matrix4x4::Transform(upperPart_, worldTransform_.parentMatrix_);

	// 旗の下部分
	lowerPart_ = { 0.0f,6.0f,0.0f };
	lowerPart_ = Matrix4x4::Transform(lowerPart_, worldTransform_.parentMatrix_);

	dxCommon_ = DirectXCommon::GetInstance();

	div_ = Vector2{ 15.0f, 15.0f };

	scale_ = Vector2{ 4.5f, 3.0f };

	cloth_ = std::make_unique<ClothGPU>();
	cloth_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), scale_, div_, "Resources/default/clothDemo.png");

	isPosSet_ = true;

}

void FlagPole::Update()
{

	BaseObstacle::Update();

	cloth_->Update(dxCommon_->GetCommadList());

	if (isPosSet_) {
		FlagPosition();
		isPosSet_ = false;
	}

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distributionX(0.0f, 100.0f);
	std::uniform_real_distribution<float> distributionZ(-20.0f, 0.0f);

	Vector3 wind = { distributionX(randomEngine), 0.0f, distributionZ(randomEngine) };

	cloth_->SetWind(wind);

}

void FlagPole::Draw(BaseCamera& camera)
{

	BaseObstacle::Draw(camera);

	cloth_->Draw(dxCommon_->GetCommadList(), &camera);

}

void FlagPole::FlagPosition()
{

	// 
	for (uint32_t y = 0; y <= static_cast<uint32_t>(div_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(div_.x); ++x) {
			cloth_->SetWeight(y, x, true);
			cloth_->SetPosition(y, x, upperPart_);
		}
	}

	// 固定する
	cloth_->SetWeight(0, 0, false);
	cloth_->SetPosition(0, 0, upperPart_);
	cloth_->SetWeight(1, 0, false);
	cloth_->SetPosition(1, 0, upperPart_);
	cloth_->SetWeight(2, 0, false);
	cloth_->SetPosition(2, 0, upperPart_);

	cloth_->SetWeight(static_cast<uint32_t>(div_.y), 0, false);
	cloth_->SetPosition(static_cast<uint32_t>(div_.y), 0, lowerPart_);
	cloth_->SetWeight(static_cast<uint32_t>(div_.y) - 1, 0, false);
	cloth_->SetPosition(static_cast<uint32_t>(div_.y) - 1, 0, lowerPart_);
	cloth_->SetWeight(static_cast<uint32_t>(div_.y) - 2, 0, false);
	cloth_->SetPosition(static_cast<uint32_t>(div_.y) - 2, 0, lowerPart_);

}
