#include "PlayerMuffler.h"
#include "../../Character/Player/Player.h"

void PlayerMuffler::Initialize(LevelData::MeshData* data)
{

	// アンカーの初期ポジション
	anchorInitPosition_ = { 0.0f,0.0f,0.0f };

	// 自然長
	naturalLength_ = 0.1f;

	// 剛性。バネ定数k
	stiffness_ = 750.0f;

	// 減衰係数
	dampingCoefficient_ = 2.0f;

	// 質量(質点)
	mass_ = 0.5f;

	// プレイヤーからのローカル位置
	localPosition_ = { 0.0f,0.0f,0.0f };

	worldTransform_.transform_.translate = localPosition_;

	BaseStringObject::Initialize(data);

}

void PlayerMuffler::Update()
{

	BaseStringObject::Update();

}

void PlayerMuffler::SetParent(Player* player)
{

	assert(player);

	player_ = player;

	worldTransform_.parent_ = player_->GetWorldTransformAdress();
	worldTransform_.UpdateMatrix();

	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {
		SetPosition(i, worldTransform_.GetWorldPosition());
	}

}
