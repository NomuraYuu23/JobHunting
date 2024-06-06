#include "BloadParticle.h"
#include <random>
#include "../../../Engine/Math/Ease.h"

void BloadParticle::Initialize(ParticleDesc* particleDesc)
{

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	transform_.scale = particleDesc->size;
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = particleDesc->position;

	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	color_ = redColor_;

	lifeTime_ = 0.2f;

	currentTime_ = 0.0f;

	useBillBoard_ = true;

	billBoardName_ = kBillBoardNameIndexAllAxis;

	UpdateMatrix(Matrix4x4::MakeIdentity4x4());

	isDead_ = false;

	Vector3 velocityMax = { 3.0f, 3.0f, 3.0f };
	Vector3 velocityMin = { -3.0f, -3.0f, -3.0f };
	std::uniform_real_distribution<float> velocityX(velocityMin.x, velocityMax.x);
	std::uniform_real_distribution<float> velocityY(velocityMin.y, velocityMax.y);
	std::uniform_real_distribution<float> velocityZ(velocityMin.z, velocityMax.z);

	velocity_ = { velocityX(randomEngine), velocityY(randomEngine), velocityZ(randomEngine) };

	float accelerationValue = 0.01f;

	if (velocity_.x > 0.0f) {
		acceleration_.x *= -1.0f;
	}
	if (velocity_.y > 0.0f) {
		acceleration_.y *= -1.0f;
	}
	if (velocity_.z > 0.0f) {
		acceleration_.z *= -1.0f;
	}


}

void BloadParticle::Update(const Matrix4x4& billBoardMatrix)
{

	velocity_ = Vector3::Subtract(velocity_, acceleration_);
	if (std::fabsf(velocity_.x) <= 0.1f) {
		velocity_.x = 0.0f;
	}
	if (std::fabsf(velocity_.y) <= 0.1f) {
		velocity_.y = 0.0f;
	}
	if (std::fabsf(velocity_.z) <= 0.1f) {
		velocity_.z = 0.0f;
	}

	transform_.translate = Vector3::Add(transform_.translate, velocity_);

	IParticle::Update(billBoardMatrix);

}
