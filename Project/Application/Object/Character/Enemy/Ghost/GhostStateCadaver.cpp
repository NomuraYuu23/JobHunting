#include "GhostStateCadaver.h"
#include "Ghost.h"

#include "../../../Engine/Physics/Gravity/Gravity.h"
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

}

void GhostStateCadaver::Update()
{

	RigidBodyUpdate();
	ghost_->SetReceiveCommand(false);

	alphaChangeStartTimeCount += kDeltaTime_;

	if (alphaChangeStartTimeCount >= alphaChangeStartTime) {
		alphaChangeStartTimeCount = alphaChangeStartTime;

		alpha -= alphaSpeed;

		if (alpha <= 0.0f) {
			alpha = 0.0f;
			ghost_->SetIsDead(true);
		}

		Vector4 color = { 1.0f, 1.0f, 1.0f, alpha };
		ghost_->SetMaterialColor(color);

	}

}

void GhostStateCadaver::RigidBodyUpdate()
{

	WorldTransform* worldTransform = ghost_->GetWorldTransformAdress();

	std::vector<NodeData>* nodeDatas = ghost_->GetLocalMatrixManager()->GetNodeDatasAddress();

	std::vector<Matrix4x4> localMatirxes;
	localMatirxes.resize(nodeDatas->size());

	for (uint32_t i = 0; i < nodeDatas->size(); ++i) {
		
		// ローカル行列取得
		localMatirxes[i] = nodeDatas->at(i).localMatrix;

		// 更新

		// 位置を取り出す
		Vector3 position = { localMatirxes[i].m[3][0], localMatirxes[i].m[3][1], localMatirxes[i].m[3][2]};
		localMatirxes[i].m[3][0] = 0.0f;
		localMatirxes[i].m[3][1] = 0.0f;
		localMatirxes[i].m[3][2] = 0.0f;

		// 回転させる
		Matrix4x4 a = Matrix4x4::MakeRotateXYZMatrix(Vector3{ 0.01f,0.0f,0.0f });

		localMatirxes[i] = localMatirxes[i] * a;

		localMatirxes[i].m[3][0] = position.x;
		localMatirxes[i].m[3][1] = position.y;
		localMatirxes[i].m[3][2] = position.z;

	}

	// 代入
	ghost_->GetLocalMatrixManager()->SetNodeLocalMatrix(localMatirxes);

}