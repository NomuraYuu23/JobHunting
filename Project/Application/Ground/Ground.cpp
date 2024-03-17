#include "Ground.h"

void Ground::Initialize(Model* model)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_.reset(Material::Create());
	material_->SetEnableLighting(BlinnPhongReflection);

	// ワールド変換データの初期化
	worldTransform_.Initialize(model_->GetRootNode());
	worldTransform_.UpdateMatrix();

}

void Ground::Update()
{

	worldTransform_.UpdateMatrix();

}

void Ground::Draw(BaseCamera& camera)
{

	model_->Draw(worldTransform_, camera, material_.get());

}
