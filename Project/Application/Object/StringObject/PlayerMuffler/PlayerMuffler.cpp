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
	localPosition_ = { 0.0f, 2.5f,0.0f };

	BaseStringObject::Initialize(data);

}

void PlayerMuffler::Update()
{

	SetPosition(0, player_->GetWorldTransformAdress()->GetWorldPosition() + localPosition_);
	SetAnchor(0, true);
	BaseStringObject::Update();

}

void PlayerMuffler::SetParent(Player* player)
{

	assert(player);

	player_ = player;

	for (uint32_t i = 0; i < structuralSpring_.size() + 1; ++i) {
		SetPosition(i, player_->GetWorldTransformAdress()->GetWorldPosition());
		SetAnchor(i, false);
	}

	SetAnchor(0,true);

}
