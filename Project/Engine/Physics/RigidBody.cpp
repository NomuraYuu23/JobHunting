#include "RigidBody.h"
#include "InertiaTensor.h"

void RigidBody::Initialize(float mass, const Vector3& size)
{

	// 慣性テンソル作成
	inertiaTensor = InertiaTensor::CreateRectangular(mass, size);

	// 基本姿勢での慣性テンソル作成
	basicPostureInertiaTensor = inertiaTensor;

	// 姿勢行列作成
	postureMatrix = Matrix4x4::MakeRotateXYZMatrix({ 0.0f, 0.0f, 0.0f });

	angularVelocity = { 0.0f,0.0f,0.0f }; // 角速度
	angularMomentum = { 0.0f,0.0f,0.0f }; // 角運動量

	centerOfGravityVelocity = { 0.0f,0.0f,0.0f }; // 重心位置速度

}

Vector3 RigidBody::TorqueCalc(
	const Vector3& centerOfGravity,
	const Vector3& pointOfAction,
	const Vector3& force)
{

	Vector3 result;
	Vector3 r = Vector3::Subtract(pointOfAction, centerOfGravity);
	result = Vector3::Cross(r, force);
	return result;

}

Matrix4x4 RigidBody::InertiaTensorCalc(
	const Matrix4x4& postureMatrix,
	const Matrix4x4& basicPostureInertiaTensor)
{

	Matrix4x4 result;

	Matrix4x4 transposePostureMatrix = Matrix4x4::Transpose(postureMatrix);

	result = Matrix4x4::Multiply(Matrix4x4::Multiply(postureMatrix, basicPostureInertiaTensor), transposePostureMatrix);

	return result;
}

Matrix4x4 RigidBody::PostureCalc(
	const Matrix4x4& postureMatrix,
	const Vector3& angularVelocity,
	float time)
{

	Matrix4x4 newPostureMatrix = Matrix4x4::MakeRotateXYZMatrix(Vector3::Multiply(angularVelocity, time));

	Matrix4x4 result = Matrix4x4::Multiply(newPostureMatrix, postureMatrix);
	result.m[3][3] = 1.0f;

	return result;

}

Vector3 RigidBody::AngularMomentumCalc(
	const Vector3& angularMomentum,
	const Vector3& torque,
	float time)
{

	Vector3 result;
	result = Vector3::Add(angularMomentum, Vector3::Multiply(torque, time));
	return result;

}

Vector3 RigidBody::AngularVelocityCalc(
	const Matrix4x4& inertiaTensor,
	const Vector3& angularMomentum)
{

	Vector3 result;
	result = Matrix4x4::Transform(angularMomentum, Matrix4x4::Inverse(inertiaTensor));
	return result;

}

Vector3 RigidBody::PointVelocityCalc(
	const Vector3& angularVelocity,
	const Vector3& centerOfGravityVelocity,
	const Vector3& point,
	const Vector3& centerOfGravity)
{

	// 重心から剛体上の点pointに向かうベクトル
	Vector3 r = Vector3::Subtract(point, centerOfGravity);

	return Vector3::Add(centerOfGravityVelocity, Vector3::Cross(angularVelocity, r));

}

RigidBody::Jacobian RigidBody::JacobianCalc(
	const Vector3& normalize, 
	const Vector3& point, 
	const Vector3& centerOfGravity)
{

	// 重心から剛体上の点pointに向かうベクトル
	Vector3 r = Vector3::Subtract(point, centerOfGravity);
	
	Jacobian result{};

	result.n = normalize;
	result.nR = Vector3::Cross(r, normalize);

	return result;

}

bool RigidBody::RestraintConfirmationJoint(
	const Vector3& velocityA, 
	const Vector3& velocityB)
{

	// 速度が同じなら拘束出来ている
	if (velocityA.x == velocityB.x &&
		velocityA.y == velocityB.y &&
		velocityA.z == velocityB.z) {

		return true;

	}

	return false;

}

bool RigidBody::RestraintConfirmationNoPenetration(
	const Vector3& velocityA, 
	const Vector3& velocityB, 
	const Vector3& normalizeB)
{

	// 速度と法線で内積を出す
	float dot = Vector3::Dot(normalizeB, Vector3::Subtract(velocityA, velocityB));

	// 内積が0より大きいなら拘束出来ている
	if (dot >= 0.0f) {

		return true;

	}

	return false;

}
