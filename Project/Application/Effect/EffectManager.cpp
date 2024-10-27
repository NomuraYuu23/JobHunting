#include "EffectManager.h"
#include "../../Engine/base/Texture/TextureManager.h"

// テクスチャパス
const std::string EffectManager::texturePath = "Resources/Sprite/Effect/";

// テクスチャ名前
const std::array<std::string, EffectManager::EffectTextureIndexOfCount> EffectManager::effectTextureNames_ =
{
	"bload.png"
};

void EffectManager::Initialize(Model* model)
{

	// 平面モデル
	model_ = model;

	// DirectXCommon
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// テクスチャハンドル
	for (uint32_t i = 0; i < EffectTextureIndexOfCount; ++i) {
		std::string fileName = texturePath + effectTextureNames_[i];
		effectTextureHandles_[i] = TextureManager::Load(fileName, dxCommon);
	}

	// ビルボードエフェクト
	for (uint32_t i = 0; i < BillboardEffectIndexOfCount; ++i) {
		billboardEffects_[i] = std::make_unique<BillboardEffect>();
		billboardEffects_[i]->Initialize(model_, effectTextureHandles_[i], 1.0f);
	}

}

void EffectManager::Update(BaseCamera& camera)
{

	// ビルボードエフェクト
	for (uint32_t i = 0; i < BillboardEffectIndexOfCount; ++i) {
		billboardEffects_[i]->Update(camera);
	}

}

void EffectManager::Draw(BaseCamera& camera)
{

	// ビルボードエフェクト
	for (uint32_t i = 0; i < BillboardEffectIndexOfCount; ++i) {
		billboardEffects_[i]->Draw(camera);
	}

}

BillboardEffect* EffectManager::GetBillboardEffect(BillboardEffectIndex index)
{

	// エフェクトあるか
	if (index >= BillboardEffectIndexOfCount) {
		return nullptr;
	}

	return billboardEffects_[index].get();

}
