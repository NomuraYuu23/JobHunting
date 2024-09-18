#include "Cloth.h"
#include "../Gravity.h"
#include "../../2D/ImguiManager.h"

void Cloth::Initialize(const Vector2& scale, const Vector2& div)
{

	// 大きさ
	scale_ = scale;
	// 分割
	div_ = div;

	mass = 1.0f; // 質点の質量
	stiffness_ = 100.0f; // 剛性。バネ定数k

	speedResistance_ = 0.2f; // 速度抵抗

	relaxation_ = 2; // バネフェーズの反復回数

	structuralShrink_ = 1.0f; // 構成バネ伸び抵抗
	structuralStretch_ = 1.0f; // 構成バネ縮み抵抗
	shearShrink_ = 1.0f; // せん断バネ伸び抵抗
	shearStretch_ = 1.0f; // せん断バネ縮み抵抗
	bendingShrink_ = 1.0f; // 曲げバネ伸び抵抗
	bendingStretch_ = 0.5f; // 曲げバネ縮み抵抗

	step_ = kDeltaTime_; // 1フレーム

	structuralDebugDraw_ = true; // 構成バネデバッグ描画
	shearDebugDraw_ = true; // せん断バネデバッグ描画
	bendingDebugDraw_ = true; // 曲げバネデバッグ描画

	// 質点初期化
	MassPointsInitialize();

	// バネ初期化
	SpringInitialize();

}

void Cloth::Update()
{

	// 積分フェーズ
	IntegralPhase();

	// バネフェーズ
	IntegralPhase();

}

void Cloth::Draw(BaseCamera& camera)
{

#ifdef _DEMO

	DebugDrawMap(DrawLine::GetInstance());

#endif // _DEMO

}

void Cloth::DebugDrawMap(DrawLine* drawLine)
{

	// 変数
	LineForGPU lineForGPU{};

	Vector4 color = {0.0f,0.0f,0.0f,1.0f};

	for (uint32_t i = 0; i < springs_.size(); ++i) {

		ClothSpring* spring = &springs_[i];

		if (spring->type_ == StructuralSpring) {
			if (structuralDebugDraw_) {
				color = { 0.8f, 0.0f,0.0f,1.0f };
			}
			else {
				continue;
			}
		}
		else if (spring->type_ == ShearSpring) {
			if (shearDebugDraw_) {
				color = { 0.0f, 0.0f,0.8f,1.0f };
			}
			else {
				continue;
			}
		}
		else {
			if (bendingDebugDraw_) {
				color = { 0.0f, 0.8f,0.0f,1.0f };
			}
			else {
				continue;
			}
		}

		// 位置と色(赤)を設定
		lineForGPU.position[0] = spring->point0_->position_;
		lineForGPU.position[1] = spring->point1_->position_;
		lineForGPU.color[0] = color;
		lineForGPU.color[1] = color;

		// マッピング
		drawLine->Map(lineForGPU);

	}

}

void Cloth::ImGuiDraw()
{

	ImGui::Begin("");

	ImGui::DragInt("バネの更新の反復の回数", &relaxation_, 0.2f, 1, 6);

	ImGui::DragFloat("抵抗", &speedResistance_, 0.01f, 0.0f);

	ImGui::DragFloat("バネの強度", &stiffness_, 0.1f, 1.0f);

	ImGui::DragFloat("構成バネ伸び抵抗", &structuralShrink_, 0.01f, 0.0f);
	ImGui::DragFloat("構成バネ縮み抵抗", &structuralStretch_, 0.01f, 0.0f);
	ImGui::DragFloat("せん断バネ伸び抵抗", &shearShrink_, 0.01f, 0.0f);
	ImGui::DragFloat("せん断バネ縮み抵抗", &shearStretch_, 0.01f, 0.0f);
	ImGui::DragFloat("曲げバネ伸び抵抗", &bendingShrink_, 0.01f, 0.0f);
	ImGui::DragFloat("曲げバネ縮み抵抗", &bendingStretch_, 0.01f, 0.0f);

	ImGui::Checkbox("構成バネデバッグ描画", &structuralDebugDraw_);
	ImGui::Checkbox("せん断バネデバッグ描画", &shearDebugDraw_);
	ImGui::Checkbox("曲げバネデバッグ描画", &bendingDebugDraw_);

	ImGui::End();

}

void Cloth::MassPointsInitialize()
{

	// クリア
	massPoints_.clear();

	// 登録
	ClothMassPoint masspoint{};
	for(uint32_t y = 0; y < static_cast<uint32_t>(div_.y) + 1; ++y){
		for (uint32_t x = 0; x < static_cast<uint32_t>(div_.x) + 1; ++x) {
			// 位置
			masspoint.position_.x = static_cast<float>(x) / div_.x * 2.0f;
			masspoint.position_.y = static_cast<float>(y) / div_.y * - 1.0f;
			masspoint.position_.z = 0.0f;
			masspoint.prePosition_ = masspoint.position_;
			// 重み 落ちないよう上の部分を固定
			if (y == 0) {
				masspoint.weight_ = 0.0f;
			}
			else {
				masspoint.weight_ = 1.0f;
			}
			// 登録
			massPoints_.push_back(masspoint);
		}
	}

}

void Cloth::SpringInitialize()
{

	// クリア
	springs_.clear();

	// 登録
	for (uint32_t y = 0; y < static_cast<uint32_t>(div_.y); ++y) {
		for (uint32_t x = 0; x < static_cast<uint32_t>(div_.x); ++x) {
			
			// 構成バネ
			SpringGeneration(x, y, -1, 0, StructuralSpring);
			SpringGeneration(x, y, 0, -1, StructuralSpring);

			// せん断バネ
			SpringGeneration(x, y, -1, -1, ShearSpring);
			SpringGeneration(x, y,  1, -1, ShearSpring);

			// 曲げバネ
			SpringGeneration(x, y, -2, 0, BendingSpring);
			SpringGeneration(x, y, 0, -2, BendingSpring);

		}
	}

}

void Cloth::IntegralPhase()
{

	Vector3 force{}; // 力

	// 重力
	force = {0.0f,- 9.8f, 0.0f};
	// 風力
	//force += {0.0f, 0.0f, 0.0f};

	// 変位に変換
	force = force * (step_ * step_ * 0.5f / mass);

	// 抵抗
	float resistance = 1.0f - speedResistance_ * step_;

	for (uint32_t i = 0; i < massPoints_.size(); ++i) {
		ClothMassPoint* point = &massPoints_[i];
		// 変位
		Vector3 dx = {};
		// 速度
		dx = point->position_ - point->prePosition_;
		// 前フレーム位置更新
		point->prePosition_ = point->position_;
		// 力の変位を足しこむ
		dx += force;
		// 抵抗
		dx *= resistance;

		// 位置更新
		dx *= point->weight_; // 固定されてるか
		point->position_ += dx;
	}


}

void Cloth::SpringPhase()
{

	for (uint32_t i = 0; i < relaxation_; ++i) {

		for (uint32_t j = 0; j < springs_.size(); ++j) {

			ClothSpring* spring = &springs_[j];

			// 二点が固定点
			if (spring->point0_->weight_ + spring->point1_->weight_ == 0.0f) {
				continue;
			}

			// 抵抗
			float shrink = 0.0f;
			float stretch = 0.0f;
			if (spring->type_ == StructuralSpring) {
				shrink = structuralShrink_;
				stretch = structuralStretch_;
			}
			else if (spring->type_ == ShearSpring) {
				shrink = shearShrink_;
				stretch = shearStretch_;
			}
			else {
				shrink = bendingShrink_;
				stretch = bendingStretch_;
			}

			// バネの力
			// 質点間の距離
			float distance = Vector3::Length(spring->point1_->position_ - spring->point0_->position_);
			// 力
			float force = (distance - spring->naturalLength_) * stiffness_;
			// 抵抗
			if (force >= 0.0f) {
				force *= shrink;
			}
			else {
				force *= stretch;
			}

			// 変位
			Vector3 dx = {};
			dx = spring->point1_->position_ - spring->point0_->position_;
			dx = Vector3::Normalize(dx);
			dx *= force;
			dx *= step_ * step_ * 0.5f / mass;

			// 位置更新
			Vector3 dx0 = {};
			dx0 = dx * (spring->point0_->weight_ / (spring->point0_->weight_ + spring->point1_->weight_));
			spring->point0_->position_ += dx0;
			Vector3 dx1 = {};
			dx1 = dx * (spring->point1_->weight_ / (spring->point0_->weight_ + spring->point1_->weight_));
			spring->point0_->position_ -= dx0;

		}

	}

}

void Cloth::SpringGeneration(
	uint32_t x, 
	uint32_t y, 
	uint32_t offsetX, 
	uint32_t offsetY, 
	TypeOfSpring type)
{

	// 終点位置
	uint32_t targetX = x + offsetX;
	uint32_t targetY = y + offsetY;

	// 範囲内か確認
	if (targetX >= 0 &&
		targetX < static_cast<uint32_t>(div_.x) + 1 &&
		targetY >= 0 &&
		targetY < static_cast<uint32_t>(div_.y) + 1) {

		// バネ
		ClothSpring spring;
		// 質点
		uint32_t index = y * (static_cast<uint32_t>(div_.x) + 1) + x;
		spring.point0_ = &massPoints_[index];
		index = targetY * (static_cast<uint32_t>(div_.x) + 1) + targetX;
		spring.point1_ = &massPoints_[index];
		// 自然長
		spring.naturalLength_ = Vector3::Length(spring.point0_->position_ - spring.point1_->position_);
		// バネの種類
		spring.type_ = type;
		// 登録
		springs_.push_back(spring);

	}

}
