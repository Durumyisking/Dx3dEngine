#pragma once
#include "druMath.h"

namespace dru::math
{
	int SM_rand();
	void SM_srand(unsigned int _seed);

	Vector3 RotateZ(const Vector3& V, const float degree);
    float toRadian(float _degree);
	float toDegree(float _radian);

	POINT WorldToWindowPos(const Vector3& worldPos);

	Vector3 RotateVector(Vector3 _vec, float _degree);


	float GetDegreeFromTwoPointZ(const Vector3& V1, const Vector3& V2);
	float GetDegreeFromTwoPointZ_0180(const Vector3& V1, const Vector3& V2);

	Vector3 ProjectOnPlane(Vector3 _Direction, Vector3 _PlaneNormal);
	Vector3 AdjustDirectionToSlope(Vector3 _Direction, Vector3 _PlaneNormal);

	int GetRandomNumber(int _Range, int _StartNumber = 0);

	float GetAngleFromDirection(Vector3 _Direction, Vector3 _Basis);

	float RotateToHead_360(Vector3 _Direction);

	template<typename T>
	T Interpolation(float _Start, float _End, float _Elapsed, T _ResultStart, T _ResultEnd)
	{
		float t = (_Elapsed - _Start) / (_End - _Start);
		T Result = static_cast<T>(_ResultStart + (_ResultEnd - _ResultStart) * t);

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

}