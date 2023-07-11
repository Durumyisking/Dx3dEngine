#pragma once
#include "druMath.h"

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