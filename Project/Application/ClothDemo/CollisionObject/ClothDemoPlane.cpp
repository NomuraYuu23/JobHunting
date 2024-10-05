#include "ClothDemoPlane.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/2D/ImguiManager.h"

void ClothDemoPlane::Initialize()
{

    // ファイル名前
    fileName_ = "plane.obj";

    // ディレクトリパス
    directoryPath_ = "Resources/default/";

    // モデル
    model_.reset(Model::Create(directoryPath_, fileName_, DirectXCommon::GetInstance()));

    // マテリアル
    material_.reset(Material::Create());

    // トランスフォーム
    worldTransform_.Initialize(model_->GetRootNode());

    // データ
    data_.distance_ = 0.0f;
    data_.normal_ = {0.0f, 1.0f, 0.0f};
    
    // 画面ちらつかないようの値
    screenDoesNotFlickerValue_ = 0.01f;
    
    // 大きさ
    worldTransform_.transform_.scale = kScale_;

}

void ClothDemoPlane::Update()
{

    // 回転
    worldTransform_.usedDirection_ = true;
    worldTransform_.direction_ = data_.normal_;
    
    // 位置
    worldTransform_.transform_.translate = data_.normal_ * (data_.distance_ - screenDoesNotFlickerValue_);

    // 行列更新
    worldTransform_.UpdateMatrix();

}

void ClothDemoPlane::Draw(BaseCamera& camera)
{

    ModelDraw::NormalObjectDesc desc;

    desc.model = model_.get();
    desc.material = material_.get();
    desc.camera = &camera;
    desc.worldTransform = &worldTransform_;

    ModelDraw::NormalObjectDraw(desc);

}

void ClothDemoPlane::ImGuiDraw()
{

    ImGui::Text("plane");
    // 法線
    ImGui::DragFloat3("plane.normal", &data_.normal_.x, 0.01f);
    data_.normal_ = Vector3::Normalize(data_.normal_);
    // 距離
    ImGui::DragFloat("plane.distance", &data_.distance_, 0.01f);

}
