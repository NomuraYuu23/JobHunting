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

	Vector3 distance = { 0.0f,-naturalLengthY_,0.0f };

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
		naturalLengthY_,
		stiffness_,
		dampingCoefficient_);
	structuralSpring_[0].SetName(localMatrixNames[kExtraMatrixNum]);

	if (structuralSpring_[0].GetAxis() == "X") {
		structuralSpring_[0].SetNaturalLength(naturalLengthX_);
	}

	sizeY_ = structuralSpring_[0].GetY();
	sizeX_ = structuralSpring_[0].GetX();

	for (uint32_t i = 1; i < structuralSpring_.size(); ++i) {
		structuralSpring_[i].Initialize(
			initMassPoint,
			initMassPoint1,
			naturalLengthY_,
			stiffness_,
			dampingCoefficient_);
		structuralSpring_[i].SetName(localMatrixNames[kExtraMatrixNum + static_cast<std::vector<std::string, std::allocator<std::string>>::size_type>(i)]);
		
		if (structuralSpring_[i].GetAxis() == "X") {
			structuralSpring_[i].SetNaturalLength(naturalLengthX_);
		}

		if (sizeY_ < structuralSpring_[i].GetY()) {
			sizeY_ = structuralSpring_[i].GetY();
		}

		if (sizeX_ < structuralSpring_[i].GetX()) {
			sizeX_ = structuralSpring_[i].GetX();
		}
	
	}

	// 位置と親を設定
	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

		// 名前取得
		std::string name = structuralSpring_[i].GetName();
		
		std::string kinds = structuralSpring_[i].GetKinds();
		std::string axis = structuralSpring_[i].GetAxis();
		uint32_t y = structuralSpring_[i].GetY();
		uint32_t x = structuralSpring_[i].GetX();

		// 位置
		initMassPoint.position = anchorInitPosition_;
		initMassPoint1.position = anchorInitPosition_;

		if (axis == "X") {
			initMassPoint.position.y += -naturalLengthY_ * static_cast<float>(y);
			initMassPoint.position.x += naturalLengthX_ * static_cast<float>(x);
			initMassPoint1.position.y += -naturalLengthY_ * static_cast<float>(y);
			initMassPoint1.position.x += naturalLengthX_ * static_cast<float>(x + 1);
		}
		else {
			initMassPoint.position.y += -naturalLengthY_ * static_cast<float>(y);
			initMassPoint.position.x += naturalLengthX_ * static_cast<float>(x);
			initMassPoint1.position.y += -naturalLengthY_ * static_cast<float>(y + 1);
			initMassPoint1.position.x += naturalLengthX_ * static_cast<float>(x);
		}

		structuralSpring_[i].SetPoint0(initMassPoint);
		structuralSpring_[i].SetPoint1(initMassPoint1);

		// 親
		// 親がいる
		if (!(y == 0 && x == 0)) {

			// 親の名前設定
			std::string parentName = "";

			// axis == Y && y == 0
			if (axis == "Y" && y == 0) {
				x--;
				axis = "X";
			}
			// axis == X && x == 0
			else if (axis == "X" && x == 0) {
				y--;
				axis = "Y";
			}
			else {
				if (axis == "X") {
					x--;
				}
				else {
					y--;
				}
			}
			
			// 親の名前設定
			parentName = kinds + "_" + axis + "_" + std::to_string(y) + "_" + std::to_string(x);

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
		structuralSpring_[i].Update(wind_);
	}

	// ずれを直す

	for (uint32_t y = 0; y <= sizeY_; ++y) {
		for (uint32_t x = 0; x <= sizeX_; ++x) {
	

			MassPoint massPointTmp{};

			StructuralSpring* downStructuralSpring = nullptr;
			MassPoint downMassPointTmp{};
			StructuralSpring* rightStructuralSpring = nullptr;
			MassPoint rightMassPointTmp{};
			StructuralSpring* upStructuralSpring = nullptr;
			MassPoint upMassPointTmp{};
			StructuralSpring* leftStructuralSpring = nullptr;
			MassPoint leftMassPointTmp{};

			// ポインタを埋める
			for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

				if (structuralSpring_[i].GetY() == y &&
					structuralSpring_[i].GetX() == x &&
					structuralSpring_[i].GetAxis() == "Y") {

					downStructuralSpring = &structuralSpring_[i];
					downMassPointTmp = structuralSpring_[i].GetPoint0();

				}
				else if (
					structuralSpring_[i].GetY() == y &&
					structuralSpring_[i].GetX() == x &&
					structuralSpring_[i].GetAxis() == "X") {
				
					rightStructuralSpring = &structuralSpring_[i];
					rightMassPointTmp = structuralSpring_[i].GetPoint0();
				
				}
				else if (
					structuralSpring_[i].GetY() == y - 1 &&
					structuralSpring_[i].GetX() == x &&
					structuralSpring_[i].GetAxis() == "Y") {

					upStructuralSpring = &structuralSpring_[i];
					upMassPointTmp = structuralSpring_[i].GetPoint1();

				}
				else if (
					structuralSpring_[i].GetY() == y &&
					structuralSpring_[i].GetX() == x - 1 &&
					structuralSpring_[i].GetAxis() == "X") {
					
					leftStructuralSpring = &structuralSpring_[i];
					leftMassPointTmp = structuralSpring_[i].GetPoint1();

				}
			}

			// ずれ修正
			float weight = 1.0f;
			float structuralSpringNum = 0;
			if (downStructuralSpring) {

				massPointTmp.position += downMassPointTmp.position;
				massPointTmp.acceleration += downMassPointTmp.acceleration;
				massPointTmp.velocity += downMassPointTmp.velocity;
				massPointTmp.force += downMassPointTmp.force;
				massPointTmp.mass += downMassPointTmp.mass;

				structuralSpringNum += 1.0f;

			}

			if (rightStructuralSpring) {

				massPointTmp.position += rightMassPointTmp.position;
				massPointTmp.acceleration += rightMassPointTmp.acceleration;
				massPointTmp.velocity += rightMassPointTmp.velocity;
				massPointTmp.force += rightMassPointTmp.force;
				massPointTmp.mass += rightMassPointTmp.mass;

				structuralSpringNum += 1.0f;

			}

			if (upStructuralSpring) {

				massPointTmp.position += upMassPointTmp.position;
				massPointTmp.acceleration += upMassPointTmp.acceleration;
				massPointTmp.velocity += upMassPointTmp.velocity;
				massPointTmp.force += upMassPointTmp.force;
				massPointTmp.mass += upMassPointTmp.mass;

				structuralSpringNum += 1.0f;

			}

			if (leftStructuralSpring) {

				massPointTmp.position += leftMassPointTmp.position;
				massPointTmp.acceleration += leftMassPointTmp.acceleration;
				massPointTmp.velocity += leftMassPointTmp.velocity;
				massPointTmp.force += leftMassPointTmp.force;
				massPointTmp.mass += leftMassPointTmp.mass;

				structuralSpringNum += 1.0f;

			}

			weight /= structuralSpringNum;

			massPointTmp.position *= weight;
			massPointTmp.acceleration *= weight;
			massPointTmp.velocity *= weight;
			massPointTmp.force *= weight;
			massPointTmp.mass *= weight;

			if (downStructuralSpring) {
				downStructuralSpring->SetPoint0(massPointTmp);
			}
			if (rightStructuralSpring) {
				rightStructuralSpring->SetPoint0(massPointTmp);
			}
			if (upStructuralSpring) {
				upStructuralSpring->SetPoint1(massPointTmp);
			}
			if (leftStructuralSpring) {
				leftStructuralSpring->SetPoint1(massPointTmp);
			}

		}
	}

	// 行列計算
	std::vector<Matrix4x4> matrixes;
	matrixes.resize(localMatrixManager_->GetNum());

	// 余分な部分
	for (uint32_t i = 0; i < kExtraMatrixNum; ++i) {
		matrixes[i] = Matrix4x4::MakeIdentity4x4();
	}
	// 基礎位置
	Vector3 basePosition{};

	for (uint32_t i = kExtraMatrixNum; i < matrixes.size(); ++i) {

		StructuralSpring* parent = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - kExtraMatrixNum].GetParent();

		if (parent) {
			basePosition = parent->GetPoint0().position;
			matrixes[i] = Matrix4x4::MakeTranslateMatrix(structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - kExtraMatrixNum].GetPoint0().position - basePosition);
		}
		else {
			matrixes[i] = Matrix4x4::MakeIdentity4x4();
		}

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

#ifdef _DEMO

	DebugDrawMap(DrawLine::GetInstance());

#endif // _DEMO

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
