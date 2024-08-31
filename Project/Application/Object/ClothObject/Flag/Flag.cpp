#include "Flag.h"

void Flag::Initialize(LevelData::MeshData* data)
{

	// アンカーの初期ポジション
	anchorInitPosition_ = { 0.0f,0.0f,0.0f };

	// 自然長
	naturalLengthY_ = 1.0f;
	naturalLengthX_ = 1.5f;

	// 剛性。バネ定数k
	stiffness_ = 750.0f;

	// 減衰係数
	dampingCoefficient_ = 2.0f;

	// 質量(質点)
	mass_ = 0.5f;

	BaseClothObject::Initialize(data);

	// ローカル位置
	//localPosition_ = { 0.0f, 2.5f,0.0f };

}

void Flag::Update()
{

	SetPosition(0, 0, worldTransform_.GetWorldPosition());
	SetAnchor(0, 0, true);
	SetPosition(4, 0, worldTransform_.GetWorldPosition() + Vector3{0.0f, -naturalLengthY_ * 4.0f, 0.0f });
	SetAnchor(4, 0, true);
	
	SetPosition(0, 4, worldTransform_.GetWorldPosition() + Vector3{ naturalLengthX_ * 4.0f, 0.0f, 0.0f });
	SetAnchor(0, 4, true);
	SetPosition(4, 4, worldTransform_.GetWorldPosition() + Vector3{ naturalLengthX_ * 4.0f, -naturalLengthY_ * 4.0f, 0.0f });
	SetAnchor(4, 4, true);
	BaseClothObject::Update();

}
