#pragma once
#include "druMath.h"

#include "../External/PhysX/Include/PxPhysicsAPI.h"


#ifdef _DEBUG


#pragma comment(lib, "../External/PhysX/lib/debug/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCharacterKinematic_static_64.lib")
#else


#pragma comment(lib, "../External/PhysX/lib/release/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCharacterKinematic_static_64.lib")
#endif

using namespace physx;


namespace dru::math
{
	int SM_rand();
	void SM_srand(unsigned int seedValue);

	Vector3 RotateZ(const Vector3& v, const float degree);
    float toRadian(float degree);
	float toDegree(float radian);

	POINT WorldToWindowPos(const Vector3& worldPos);

	Vector3 RotateVector(Vector3 vec, float degree);


	float GetDegreeFromTwoPointZ(const Vector3& v1, const Vector3& v2);
	float GetDegreeFromTwoPointZ_0180(const Vector3& v1, const Vector3& v2);

	Vector3 ProjectOnPlane(Vector3 direction, Vector3 planeNormal);
	Vector3 AdjustDirectionToSlope(Vector3 direction, Vector3 planeNormal);

	int GetRandomNumber(int range, int startNumber = 0);

	float GetAngleFromDirection(Vector3 direction, Vector3 basis);

	float RotateToHead_360(Vector3 direction);

	template<typename T>
	T Interpolation(float start, float end, float elapsed, T resultStart, T resultEnd)
	{
		float t = (elapsed - start) / (end - start);
		T Result = static_cast<T>(resultStart + (resultEnd - resultStart) * t);

		return Result;
	}
}

namespace dru::convert
{
	using namespace dru::math;

	static PxVec3 Vector3ToPxVec3(Vector3 vector3)
	{
		return PxVec3(vector3.x, vector3.y, vector3.z);
	}

	static PxExtendedVec3 Vector3ToPxExtendedVec3(Vector3 vector3)
	{
		return PxExtendedVec3(vector3.x, vector3.y, vector3.z);
	}

	static PxVec4 Vec4ToPxVec4(Vector4 vector4)
	{
		return PxVec4(vector4.x, vector4.y, vector4.z, vector4.w);
	}

	static PxMat44 Mat44ToPxMat44(Matrix matMatrix)
	{
		PxMat44 pxMat44 = {};
		pxMat44.column0 = PxVec4(matMatrix._11, matMatrix._12, matMatrix._13, matMatrix._14);
		pxMat44.column1 = PxVec4(matMatrix._21, matMatrix._22, matMatrix._23, matMatrix._24);
		pxMat44.column2 = PxVec4(matMatrix._31, matMatrix._32, matMatrix._33, matMatrix._34);
		pxMat44.column3 = PxVec4(matMatrix._41, matMatrix._42, matMatrix._43, matMatrix._44);

		return pxMat44;
	}

	static Vector3 PxVec3ToVector3(PxVec3 pxVec3)
	{
		return Vector3(pxVec3.x, pxVec3.y, pxVec3.z);
	}

	static Vector3 PxExtendedVec3ToVector3(PxExtendedVec3 pxExVector3)
	{
		return Vector3(
			static_cast<float>(pxExVector3.x),
			static_cast<float>(pxExVector3.y),
			static_cast<float>(pxExVector3.z));
	}

	static Vector4 PxVec4ToVector4(PxVec4 PxVec3)
	{
		return Vector4(PxVec3.x, PxVec3.y, PxVec3.z, PxVec3.w);
	}

	static Matrix PxMat44ToMat44(PxMat44 pxMatrix)
	{
		Matrix mat = {};
		mat._11 = pxMatrix.column0.x;
		mat._12 = pxMatrix.column0.y;
		mat._13 = pxMatrix.column0.z;
		mat._14 = pxMatrix.column0.w;

		mat._21 = pxMatrix.column1.x;
		mat._22 = pxMatrix.column1.y;
		mat._23 = pxMatrix.column1.z;
		mat._24 = pxMatrix.column1.w;

		mat._31 = pxMatrix.column2.x;
		mat._32 = pxMatrix.column2.y;
		mat._33 = pxMatrix.column2.z;
		mat._34 = pxMatrix.column2.w;

		mat._41 = pxMatrix.column3.x;
		mat._42 = pxMatrix.column3.y;
		mat._43 = pxMatrix.column3.z;
		mat._44 = pxMatrix.column3.w;

		return mat;
	}
	static Quaternion PxQuatToQuaternion(const PxQuat& pxQuat)
	{
		return Quaternion(pxQuat.x, pxQuat.y, pxQuat.z, pxQuat.w);
	}

	void SetRandIndex(std::wstring& string, UINT range);
}