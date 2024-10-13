#include "ClothDemoCapsule.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Texture/TextureManager.h"

void ClothDemoCapsule::Initialize(const std::string& name)
{


    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    // ファイル名前
    fileName_ = "ball.obj";

    // ディレクトリパス
    directoryPath_ = "Resources/default/";

    // モデル
    model_.reset(Model::Create(directoryPath_, fileName_, dxCommon));

    // マテリアル
    material_.reset(Material::Create());
    material_->SetColor(Vector4{ 0.5f,1.0f,0.5f,1.0f });
    material_->SetEnableLighting(HalfLambert);

    // トランスフォーム
    worldTransform_.Initialize(model_->GetRootNode());

    // データ
    data_.origin_ = { 0.0f, 0.0f, 0.0f };
    data_.diff_ = { 0.0f, 1.0f, 0.0f };
    data_.radius_ = 0.5f;

    // 画面ちらつかないようの値
    screenDoesNotFlickerValue_ = 0.01f;

    // デモに存在するか
    exist_ = false;

    // 名前
    name_ = name;

    // テクスチャハンドル
    textureHandle_ = TextureManager::Load("Resources/default/white2x2.png", dxCommon);

    // トランスフォーム、カプセルの下用
    diffWorldTransform_.Initialize(model_->GetRootNode());

}

void ClothDemoCapsule::Update()
{

    // 位置
    worldTransform_.transform_.translate = data_.origin_;
    float size = data_.radius_ - screenDoesNotFlickerValue_;
    worldTransform_.transform_.scale = { size, size, size };

    // 行列更新
    worldTransform_.UpdateMatrix();

    // 位置
    diffWorldTransform_.transform_.translate = data_.origin_ + data_.diff_;
    diffWorldTransform_.transform_.scale = { size, size, size };

    // 行列更新
    diffWorldTransform_.UpdateMatrix();

}

void ClothDemoCapsule::Draw(BaseCamera& camera)
{

    // 出現してないなら描画しない
    if (!exist_) {
        return;
    }

    ModelDraw::NormalObjectDesc desc;
    std::vector<UINT> textureHandles;
    textureHandles.push_back(textureHandle_);

    desc.model = model_.get();
    desc.material = material_.get();
    desc.camera = &camera;
    desc.worldTransform = &worldTransform_;
    desc.textureHandles = textureHandles;

    ModelDraw::NormalObjectDraw(desc);

    desc.worldTransform = &diffWorldTransform_;
    ModelDraw::NormalObjectDraw(desc);

}

void ClothDemoCapsule::ImGuiDraw()
{

    ImGui::Text("capsule");
    // 原点
    ImGui::DragFloat3("sphere.origin", &data_.origin_.x, 0.01f);
    // 終点までのベクトル
    ImGui::DragFloat3("sphere.diff", &data_.diff_.x, 0.01f);
    // 距離
    ImGui::DragFloat("sphere.radius", &data_.radius_, 0.01f);

}
