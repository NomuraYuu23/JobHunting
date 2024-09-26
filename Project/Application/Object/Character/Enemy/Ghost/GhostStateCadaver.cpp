#include "GhostStateCadaver.h"
#include "Ghost.h"

#include "../../../Engine/Physics/Gravity.h"
#include "../../../Engine/Math/DeltaTime.h"

void GhostStateCadaver::Initialize()
{

	ghostStateNo_ = GhostState::kGhostStateCadaver;

	ghostMotionNo_ = kGhostMotionCadaver;

	// コライダー調整
	OBB obb = std::get<OBB>(*ghost_->GetCollider());

	float tmp = obb.size_.y;
	obb.size_.y = obb.size_.x;
	obb.size_.x = tmp;
	obb.size_.z = tmp;

	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	ghost_->SetCollider(colliderShape);


	const uint32_t kExtraMatrixNum = 2;
	LocalMatrixManager* localMatrixManager = ghost_->GetLocalMatrixManager();

	// バネの初期化
	structuralSpring_.resize(localMatrixManager->GetNum() - kExtraMatrixNum);

	Vector3 distance = { 0.0f, 0.0f, 0.0f };
	Vector3 postion0 = { 0.0f, 0.0f, 0.0f };
	Vector3 postion1 = { 0.0f, 0.0f, 0.0f };

	MassPoint initMassPoint;
	initMassPoint.position = { 0.0f, 0.0f, 0.0f };
	initMassPoint.mass = mass_;
	initMassPoint.acceleration = { 0.0f,0.0f,0.0f };
	initMassPoint.velocity = { 0.0f,0.0f,0.0f };
	initMassPoint.force = { 0.0f,0.0f,0.0f };
	MassPoint initMassPoint1 = initMassPoint;

	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

		// ポイント0
		postion0 = Matrix4x4::Transform({ 0.0f, 0.0f, 0.0f }, localMatrixManager->GetNodeDatas()[kExtraMatrixNum + i].matrix);

		// ポイント1
		postion1 = { 0.0f,0.0f,0.0f };
		uint32_t childerenSize = static_cast<uint32_t>(localMatrixManager->GetNodeDatas()[kExtraMatrixNum + i].childrenIndexes.size());
		for (uint32_t j = 0; j < childerenSize; ++j) {
			uint32_t childrenIndex = localMatrixManager->GetNodeDatas()[kExtraMatrixNum + i].childrenIndexes[j];
			postion1 += Matrix4x4::Transform({ 0.0f, 0.0f, 0.0f }, localMatrixManager->GetNodeDatas()[childrenIndex].matrix);
		}
		postion1 *= 1.0f / static_cast<float>(childerenSize);

		//naturalLength
		float naturalLength = Vector3::Length(postion0 - postion1);

		initMassPoint.position = postion0;
		initMassPoint1.position = postion1;
		structuralSpring_[i].Initialize(
			initMassPoint,
			initMassPoint1,
			naturalLength,
			stiffness_,
			dampingCoefficient_);
	}

	structuralSpring_[0].SetFixPoint0(true);

}

void GhostStateCadaver::Update()
{

	//// 変数
	//std::vector<StructuralSpring> spring = structuralSpring_; // バネ

	////0番目更新
	//structuralSpring_[0].SetPoint1(spring[1].GetPoint0());
	//structuralSpring_[0].Update();

	//// それ以外を更新
	//for (uint32_t i = 1; i < structuralSpring_.size() - 1; ++i) {
	//	structuralSpring_[i].SetPoint0(spring[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - 1].GetPoint1());
	//	structuralSpring_[i].SetPoint1(spring[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) + 1].GetPoint0());
	//	structuralSpring_[i].Update();
	//}

	////最後を更新
	//structuralSpring_[structuralSpring_.size() - 1].SetPoint0(spring[spring.size() - 2].GetPoint1());
	//structuralSpring_[structuralSpring_.size() - 1].Update();

	//// ずれを直す
	//MassPoint massPointTmp;
	//MassPoint massPoint1Tmp;
	//MassPoint massPoint0Tmp;
	//for (uint32_t i = 0; i < structuralSpring_.size() - 1; ++i) {

	//	structuralSpring_[i].PositionLimit();

	//	massPoint1Tmp = structuralSpring_[i].GetPoint1();
	//	massPoint0Tmp = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) + 1].GetPoint0();

	//	massPointTmp.position = (massPoint1Tmp.position + massPoint0Tmp.position) * 0.5f;
	//	massPointTmp.acceleration = (massPoint1Tmp.acceleration + massPoint0Tmp.acceleration) * 0.5f;
	//	massPointTmp.velocity = (massPoint1Tmp.velocity + massPoint0Tmp.velocity) * 0.5f;
	//	massPointTmp.force = (massPoint1Tmp.force + massPoint0Tmp.force) * 0.5f;
	//	massPointTmp.mass = (massPoint1Tmp.mass + massPoint0Tmp.mass) * 0.5f;

	//	structuralSpring_[i].SetPoint1(massPointTmp);
	//	structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) + 1].SetPoint0(massPointTmp);

	//}

	// 行列計算

	const uint32_t kExtraMatrixNum = 2;
	LocalMatrixManager* localMatrixManager = ghost_->GetLocalMatrixManager();

	std::vector<Matrix4x4> matrixes;
	matrixes.resize(localMatrixManager->GetNum());

	// 余分な部分
	for (uint32_t i = 0; i < kExtraMatrixNum; ++i) {
		matrixes[i] = Matrix4x4::MakeIdentity4x4();
	}

	for (uint32_t i = kExtraMatrixNum; i < matrixes.size(); ++i) {
		StructuralSpring spring = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - kExtraMatrixNum];
		
		
		Vector3 dir = Vector3::Normalize(spring.GetPoint1().position - spring.GetPoint0().position);

		Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(spring.GetPoint0().position);
		Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection({0.0f,0.0f,1.0f}, dir);

		matrixes[i] = Matrix4x4::Multiply(rotateMatrix, translateMatrix);

	}

	// ワールドトランスフォーム
	localMatrixManager->SetNodeLocalMatrix(matrixes);

	//localMatrixManager->Map();
	//ghost_->GetWorldTransformAdress()->transform_.translate = structuralSpring_[0].GetPoint0().position;
	ghost_->GetWorldTransformAdress()->UpdateMatrix();

}
