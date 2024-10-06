#include "ClothDemoSphere.h"
#include "../../../Engine/2D/ImguiManager.h"

void ClothDemoSphere::Initialize(const std::string& name)
{

    // ファイル名前
    fileName_ = "ball.obj";

    // ディレクトリパス
    directoryPath_ = "Resources/default/";

    // モデル
    model_.reset(Model::Create(directoryPath_, fileName_, DirectXCommon::GetInstance()));

    // マテリアル
    material_.reset(Material::Create());

    // トランスフォーム
    worldTransform_.Initialize(model_->GetRootNode());

    // データ
    data_.position_ = { 0.0f, 0.0f, 0.0f };
    data_.radius_ = 0.5f;

    // 画面ちらつかないようの値
    screenDoesNotFlickerValue_ = 0.01f;

    // デモに存在するか
    exist_ = false;

    // 名前
    name_ = name;

}

void ClothDemoSphere::Update()
{

    // 位置
    worldTransform_.transform_.translate = data_.position_;
    float size = data_.radius_ - screenDoesNotFlickerValue_;
    worldTransform_.transform_.scale = { size, size, size };

    // 行列更新
    worldTransform_.UpdateMatrix();

}

void ClothDemoSphere::ImGuiDraw()
{

    ImGui::Text("sphere");
    // 法線
    ImGui::DragFloat3("sphere.position", &data_.position_.x, 0.01f);
    // 距離
    ImGui::DragFloat("sphere.radius", &data_.radius_, 0.01f);

}
