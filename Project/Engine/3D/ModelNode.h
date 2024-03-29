#pragma once
#include "../Math/Matrix4x4.h"
#include <string>
#include <vector>

class ModelNode
{

public:

	Matrix4x4 localMatrix;
	Matrix4x4 offsetMatrix;
	std::string name;
	std::vector<ModelNode> children;
	uint32_t meshNum;

};

