#include "LocalMatrixDraw.h"

void LocalMatrixDraw::DrawMap(
	LocalMatrixManager* localMatrixManager,
	WorldTransform* worldTransform,
	DrawLine* drawLine)
{

	LineForGPU lineForGPU;

	for (uint32_t i = 0; i < localMatrixManager->GetNum(); ++i) {
		// 親
		int32_t parentIndex = localMatrixManager->GetNodeDatasAddress()->at(i).parentIndex;
		if (parentIndex == -1) {
			// 親ないので帰る
			continue;
		}
		Vector3 startPosition = {
		localMatrixManager->GetNodeDatasAddress()->at(parentIndex).matrix.m[3][0],
		localMatrixManager->GetNodeDatasAddress()->at(parentIndex).matrix.m[3][1],
		localMatrixManager->GetNodeDatasAddress()->at(parentIndex).matrix.m[3][2] };

		lineForGPU.position[0] = Matrix4x4::Transform(startPosition, worldTransform->worldMatrix_);

		// 本体
		Vector3 endPosition = {
		localMatrixManager->GetNodeDatasAddress()->at(i).matrix.m[3][0],
		localMatrixManager->GetNodeDatasAddress()->at(i).matrix.m[3][1],
		localMatrixManager->GetNodeDatasAddress()->at(i).matrix.m[3][2] };

		lineForGPU.position[1] = Matrix4x4::Transform(endPosition, worldTransform->worldMatrix_);

		lineForGPU.color[0] = Vector4{ 1.0f,1.0f,1.0f,1.0f };
		lineForGPU.color[1] = Vector4{ 1.0f,1.0f,1.0f,1.0f };

		drawLine->Map(lineForGPU);

	}

}
