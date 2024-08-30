#include "BaseClothObject.h"
#include "../../../Engine/3D/ModelDraw.h"

void BaseClothObject::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	// ローカル行列マネージャー
	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	// ボーンの数が足りていない
	assert(localMatrixManager_->GetNum() > kExtraMatrixNum + 1);

	// アンカーの初期ポジション
	anchorInitPosition_ = worldTransform_.GetWorldPosition();

	// バネの初期化
	structuralSpring_.resize(localMatrixManager_->GetNum() - kExtraMatrixNum);

	Vector3 distance = { 0.0f,-naturalLength_,0.0f };

	MassPoint initMassPoint;
	initMassPoint.position = anchorInitPosition_;
	initMassPoint.mass = mass_;
	initMassPoint.acceleration = { 0.0f,0.0f,0.0f };
	initMassPoint.velocity = { 0.0f,0.0f,0.0f };
	initMassPoint.force = { 0.0f,0.0f,0.0f };
	MassPoint initMassPoint1;
	initMassPoint1.position = Vector3::Add(anchorInitPosition_, distance);
	initMassPoint1.mass = mass_;
	initMassPoint1.acceleration = { 0.0f,0.0f,0.0f };
	initMassPoint1.velocity = { 0.0f,0.0f,0.0f };
	initMassPoint1.force = { 0.0f,0.0f,0.0f };

	// バネの初期化
	std::vector<std::string> localMatrixNames = localMatrixManager_->GetNodeNames();

	// 基本設定
	structuralSpring_[0].Initialize(
		initMassPoint,
		initMassPoint1,
		naturalLength_,
		stiffness_,
		dampingCoefficient_);
	structuralSpring_[0].SetName(localMatrixNames[kExtraMatrixNum]);

	for (uint32_t i = 1; i < structuralSpring_.size(); ++i) {
		structuralSpring_[i].Initialize(
			initMassPoint,
			initMassPoint1,
			naturalLength_,
			stiffness_,
			dampingCoefficient_);
		structuralSpring_[0].SetName(localMatrixNames[kExtraMatrixNum + static_cast<std::vector<std::string, std::allocator<std::string>>::size_type>(i)]);
	}

	// 位置と親を設定
	std::string kinds = "";
	std::string axis = "";
	std::string y = "";
	std::string x = "";
	size_t tmpIndex = 0;

	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

		// 名前取得
		std::string name = structuralSpring_[i].GetName();

		// 名前分割

		// "_"の位置を取得
		tmpIndex = name.find_first_of("_", 0);
		// "_"のひとつ前までを代入
		kinds =	name.substr(0, tmpIndex - 1);
		// 名前を"_"の一つ先からに変更する
		name = name.substr(tmpIndex + 1);

		// "_"の位置を取得
		tmpIndex = name.find_first_of("_", 0);
		// "_"のひとつ前までを代入
		axis = name.substr(0, tmpIndex - 1);
		// 名前を"_"の一つ先からに変更する
		name = name.substr(tmpIndex + 1);

		// "_"の位置を取得
		tmpIndex = name.find_first_of("_", 0);
		// "_"のひとつ前までを代入
		y = name.substr(0, tmpIndex - 1);
		// 名前を"_"の一つ先からに変更する
		name = name.substr(tmpIndex + 1);

		// "_"の位置を取得
		tmpIndex = name.find_first_of("_", 0);
		// "_"のひとつ前までを代入
		x = name.substr(0, tmpIndex - 1);
		// 名前を"_"の一つ先からに変更する
		name = name.substr(tmpIndex + 1);

		// 位置
		initMassPoint.position = anchorInitPosition_;
		initMassPoint1.position = anchorInitPosition_;

		if (axis.c_str() == "X") {
			initMassPoint.position.y += -naturalLength_ * static_cast<float>(atoi(y.c_str()));
			initMassPoint.position.x += naturalLength_ * static_cast<float>(atoi(x.c_str()));
			initMassPoint1.position.y += -naturalLength_ * static_cast<float>(atoi(y.c_str()));
			initMassPoint1.position.x += naturalLength_ * static_cast<float>((atoi(x.c_str()) + 1));
		}
		else {
			initMassPoint.position.y += -naturalLength_ * static_cast<float>(atoi(y.c_str()));
			initMassPoint.position.x += naturalLength_ * static_cast<float>(atoi(x.c_str()));
			initMassPoint1.position.y += -naturalLength_ * static_cast<float>((atoi(y.c_str()) + 1));
			initMassPoint1.position.x += naturalLength_ * static_cast<float>(atoi(x.c_str()));
		}

		structuralSpring_[i].SetPoint0(initMassPoint);
		structuralSpring_[i].SetPoint1(initMassPoint1);

		// 親
		// 親がいる
		if (!(y.c_str() == "0" && x.c_str() == "0")) {

			// 親の名前設定
			std::string parentName = "";

			// axis == Y && y == 0
			if (axis.c_str() == "Y" && y.c_str() == "0") {
				x = std::to_string(atoi(x.c_str()) - 1);
				axis = "X";
			}
			// axis == X && x == 0
			else if (axis.c_str() == "X" && x.c_str() == "0") {
				y = std::to_string(atoi(y.c_str()) - 1);
				axis = "Y";
			}
			else {
				if (axis == "X") {
					x = std::to_string(atoi(x.c_str()) - 1);
				}
				else {
					y = std::to_string(atoi(y.c_str()) - 1);
				}
			}
			
			// 親の名前設定
			parentName = kinds + "_" + axis + "_" + y + "_" + x;

			// 親設定
			size_t parentNum = 0;
			for (uint32_t j = 0; j < structuralSpring_.size(); ++j) {

				if (structuralSpring_[j].GetName() == parentName) {
					parentNum = j;
					break;
				}

			}
			structuralSpring_[i].SetParent(&structuralSpring_[parentNum]);

		}

	}

}

void BaseClothObject::Update()
{

	MeshObject::Update();

	// 変数
	std::vector<StructuralSpring> spring = structuralSpring_; // バネ

	// それ以外を更新
	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

		StructuralSpring* parent = structuralSpring_[i].GetParent();

		if (parent) {
			structuralSpring_[i].SetPoint0(parent->GetPoint1());
		}
		structuralSpring_[i].Update();
	}

	// ずれを直す
	MassPoint massPointTmp;
	MassPoint massPoint1Tmp;
	MassPoint massPoint0Tmp;
	for (uint32_t i = 0; i < structuralSpring_.size() - 1; ++i) {

		structuralSpring_[i].PositionLimit();

		massPoint1Tmp = structuralSpring_[i].GetPoint1();
		massPoint0Tmp = structuralSpring_[i].GetParent()->GetPoint0();

		massPointTmp.position = (massPoint1Tmp.position + massPoint0Tmp.position) * 0.5f;
		massPointTmp.acceleration = (massPoint1Tmp.acceleration + massPoint0Tmp.acceleration) * 0.5f;
		massPointTmp.velocity = (massPoint1Tmp.velocity + massPoint0Tmp.velocity) * 0.5f;
		massPointTmp.force = (massPoint1Tmp.force + massPoint0Tmp.force) * 0.5f;
		massPointTmp.mass = (massPoint1Tmp.mass + massPoint0Tmp.mass) * 0.5f;

		structuralSpring_[i].SetPoint1(massPointTmp);
		structuralSpring_[i].GetParent()->SetPoint0(massPointTmp);

	}

	// 行列計算
	std::vector<Matrix4x4> matrixes;
	matrixes.resize(localMatrixManager_->GetNum());

	// 余分な部分
	for (uint32_t i = 0; i < kExtraMatrixNum; ++i) {
		matrixes[i] = Matrix4x4::MakeIdentity4x4();
	}
	// 基礎位置
	Vector3 basePosition = structuralSpring_[0].GetPoint0().position;

	for (uint32_t i = kExtraMatrixNum; i < matrixes.size(); ++i) {
		matrixes[i] = Matrix4x4::MakeTranslateMatrix(structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - kExtraMatrixNum].GetPoint0().position - basePosition);
		basePosition = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - kExtraMatrixNum].GetPoint0().position;
	}

	// ワールドトランスフォーム
	localMatrixManager_->SetNodeLocalMatrix(matrixes);

	localMatrixManager_->Map();
	worldTransform_.transform_.translate = structuralSpring_[0].GetPoint0().position;
	worldTransform_.UpdateMatrix();

}

void BaseClothObject::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

	DebugDrawMap(DrawLine::GetInstance());

}

void BaseClothObject::DebugDrawMap(DrawLine* drawLine)
{

	// 変数
	LineForGPU lineForGPU{};

	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

		// 位置と色(赤)を設定
		lineForGPU.position[0] = structuralSpring_[i].GetPoint0().position;
		lineForGPU.position[1] = structuralSpring_[i].GetPoint1().position;
		lineForGPU.color[0] = Vector4{ 0.8f,0.0f,0.0f,1.0f };
		lineForGPU.color[1] = Vector4{ 0.8f,0.0f,0.0f,1.0f };

		// マッピング
		drawLine->Map(lineForGPU);

	}

}

void BaseClothObject::SetAnchor(uint32_t y, uint32_t x, bool fixPoint)
{

	// アンカー状態を変更する
	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {
	
		if (structuralSpring_[i].GetY() == y &&
			structuralSpring_[i].GetX() == x) {

			structuralSpring_[i].SetFixPoint0(fixPoint);

		}
		else if (
			structuralSpring_[i].GetY() == y - 1 &&
			structuralSpring_[i].GetX() == x &&
			structuralSpring_[i].GetAxis() == "Y") {
			structuralSpring_[i].SetFixPoint1(fixPoint);
		}
		else if (
			structuralSpring_[i].GetY() == y &&
			structuralSpring_[i].GetX() == x - 1&&
			structuralSpring_[i].GetAxis() == "X") {
			structuralSpring_[i].SetFixPoint1(fixPoint);
		}
	}

}

void BaseClothObject::SetPosition(uint32_t y, uint32_t x, const Vector3& position)
{
	// 変数
	MassPoint massPoint;

	// アンカー状態を変更する
	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

		if (structuralSpring_[i].GetY() == y &&
			structuralSpring_[i].GetX() == x) {

			massPoint = structuralSpring_[i].GetPoint0();
			massPoint.position = position;
			structuralSpring_[i].SetPoint0(massPoint);

		}
		else if (
			structuralSpring_[i].GetY() == y - 1 &&
			structuralSpring_[i].GetX() == x &&
			structuralSpring_[i].GetAxis() == "Y") {

			massPoint = structuralSpring_[i].GetPoint1();
			massPoint.position = position;
			structuralSpring_[i].SetPoint1(massPoint);
		
		}
		else if (
			structuralSpring_[i].GetY() == y &&
			structuralSpring_[i].GetX() == x - 1 &&
			structuralSpring_[i].GetAxis() == "X") {

			massPoint = structuralSpring_[i].GetPoint1();
			massPoint.position = position;
			structuralSpring_[i].SetPoint1(massPoint);
		
		}
	}

}

void BaseClothObject::CollisionListRegister(CollisionManager* collisionManager)
{

	return;

}

void BaseClothObject::CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	return;

}