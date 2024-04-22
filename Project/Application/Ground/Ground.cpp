#include "Ground.h"
#include "../../Engine/3D/ModelDraw.h"

void Ground::Initialize(Model* model)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_.reset(Material::Create());
	material_->SetEnableLighting(BlinnPhongReflection);

	// ワールド変換データの初期化
	worldTransform_.Initialize(model_->GetRootNode());

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

}

void Ground::Update()
{

	worldTransform_.UpdateMatrix();

	localMatrixManager_->Map();

}

void Ground::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}
