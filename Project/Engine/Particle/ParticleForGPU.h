#pragma once
#include "../Math/Matrix/Matrix4x4.h"
#include "../Math/Vector/Vector4.h"

struct ParticleForGPU{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};