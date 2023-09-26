#include "SimpleMath.h"
#include "Application.h"
#include "Renderer.h"
#include "GameObj.h"

extern Application application;

namespace math
{
	static unsigned long int seed = 1;
	/* rand: return pseudo-random integer on 0..32767 */
	int SM_rand()
	{
		seed = seed * 1103515245 + 12345;
		return (unsigned int)(seed / 65536) % 32768;
	}
	/* srand: set seed for rand() */
	void SM_srand(unsigned int seedValue)
	{
		seed = seedValue;
	}

	Vector3 RotateZ(const Vector3& v, const float degree)
	{

		float rad = toRadian(degree); // ���� ������ ��ȯ
		float s = sin(rad);
		float c = cos(rad);

		Vector3 rotated = {};

		rotated.x = v.x * c - v.y * s;
		rotated.y = v.x * s + v.y * c;
		rotated.z = v.z;

		return rotated;
	}

	float toRadian(float degree)
	{
		return degree * XM_PI / 180.f;
	}

	float toDegree(float radian)
	{
		return radian * 180 / XM_PI;
	}

	POINT WorldToWindowPos(const Vector3& worldPos)
	{
		RECT windowRect;
		GetClientRect(application.GetHwnd(), &windowRect);

		Vector2 resolutionRatio = application.GetResolutionRatio();

		Vector2 windowPos;
		if (renderer::mainCamera)
		{
			Vector3 camPos = renderer::mainCamera->GetOwner()->GetWorldPos();
			windowPos.x = (worldPos.x - camPos.x) * 100.f; 
			windowPos.y = (worldPos.y - camPos.y) * -100.f; 
			//windowPos.x = (worldPos.x) * 100.f;
			//windowPos.y = (worldPos.y) * 100.f;
		}
		windowPos.x /= resolutionRatio.x;
		windowPos.y /= resolutionRatio.y;
		windowPos.x += (windowRect.right - windowRect.left) * 0.5f;
		windowPos.y += (windowRect.bottom - windowRect.top) * 0.5f;

		POINT ptResult;
		ptResult.x = static_cast<LONG>(windowPos.x);
		ptResult.y = static_cast<LONG>(windowPos.y);

		ClientToScreen(application.GetHwnd(), &ptResult);

		return ptResult;
	}

	Vector3 RotateVector(Vector3 vec, float degree)
	{
		// ȸ�� �� ���͸� �����մϴ�. �� ��� Z���� ȸ�������� ����մϴ�.
		Vector3 axis = { 0.f, 0.f, 1.f };

		// �־��� ����(theta)�� �������� ��ȯ�մϴ�.
		float radians = toRadian(degree);

		// ȸ�� ����� �����մϴ�.
		XMMATRIX rotationMatrix = XMMatrixRotationAxis(axis, radians);

		// ���͸� XMVECTOR �������� ��ȯ�մϴ�.
		XMVECTOR vecToRotate = XMLoadFloat3(&vec);

		// ȸ�� ��İ� ���͸� ���� ȸ����ŵ�ϴ�.
		XMVECTOR rotatedVec = XMVector3Transform(vecToRotate, rotationMatrix);

		// ȸ���� ���͸� Vector3 �������� �ٽ� ��ȯ�մϴ�.
		XMStoreFloat3(&vec, rotatedVec);

		return vec;
	}

	float GetDegreeFromTwoPointZ(const Vector3& v1, const Vector3& v2)
	{
		Vector3 rotation = {};

		// x��� mousepos <-> slashobj ���� ���� ����
		rotation.z = atan2(v1.y - v2.y, v1.x - v2.x);
		// radian to degree
		rotation.z = rotation.z * 180 / XM_PI;

		return rotation.z;
	}

	float GetDegreeFromTwoPointZ_0180(const Vector3& v1, const Vector3& v2)
	{
		Vector3 rotation = {};
		rotation.z = atan2(v2.x - v1.x, v2.y - v1.y) * 180 / XM_PI;

		// 0������ 180���� ��ȯ
		if (rotation.z < 0)
		{
			rotation.z *= -1.f;
		}

		return rotation.z;
	}

	Vector3 ProjectOnPlane(Vector3 direction, Vector3 planeNormal)
	{
		planeNormal.Normalize();
		
		// ���͸� ���� ������ ���ͷ� �����մϴ�.
		Vector3 verticalVector = direction.Dot(planeNormal) * planeNormal;

		// ���� ������ ���͸� ���� ���͸� ��鿡 �����մϴ�.
		Vector3 projectedVector = direction - verticalVector;
		projectedVector.z = 0.f;
		return projectedVector;
	}

	Vector3 AdjustDirectionToSlope(Vector3 direction, Vector3 planeNormal)
	{
		Vector3 Result = ProjectOnPlane(direction, planeNormal);
		//Result.Normalize();
		return Result;
	}

	int GetRandomNumber(int range, int startNumber)
	{
		SM_srand(seed);
		int randvalue = (SM_rand() % range) + startNumber;;

		return randvalue;
	}


	float GetAngleFromDirection(Vector3 direction, Vector3 basis)
	{

		// �̵� ����� forward ���� ���� ������ ���մϴ�.
		float angle = acos(direction.Dot(basis)) * 180.f / XM_PI;

		// �̵� ������ y���� ���� ���, ������ �����մϴ�.
		if (direction.y < 0.f)
		{
			angle = -angle;
		}
		return angle;
	}

	float RotateToHead_360(Vector3 direction)
	{
		Vector3 forward = { 1.f, 0.f, 0.f }; // �ʱⰪ���� x���� ����մϴ�.

		// �̵� ����� forward ���� ���� ������ ���մϴ�.
		float angle = acos(direction.Dot(forward)) * 180.f / XM_PI;

		return angle;
	}

	Vector3 DecomposeRotMat(const Matrix _matRot)
	{
		// _mat �� ���� �� �ٽ� ��� �����	
		Vector4 vMat[4];

		vMat[0] = Vector4(_matRot._11, _matRot._12, _matRot._13, _matRot._14);
		vMat[1] = Vector4(_matRot._21, _matRot._22, _matRot._23, _matRot._24);
		vMat[2] = Vector4(_matRot._31, _matRot._32, _matRot._33, _matRot._34);
		vMat[3] = Vector4(_matRot._41, _matRot._42, _matRot._43, _matRot._44);

		/*XMStoreFloat4(&vMat[0], _matRot._11);
		XMStoreFloat4(&vMat[1], _matRot.r[1]);
		XMStoreFloat4(&vMat[2], _matRot.r[2]);
		XMStoreFloat4(&vMat[3], _matRot.r[3]);*/

		Vector3 vNewRot;
		if (closeEnough(vMat[0].z, -1.0f)) {
			float x = 0; //gimbal lock, value of x doesn't matter
			float y = XM_PI / 2;
			float z = x + atan2f(vMat[1].x, vMat[2].x);
			vNewRot = Vector3{ x, y, z };
		}
		else if (closeEnough(vMat[0].z, 1.0f)) {
			float x = 0;
			float y = -XM_PI / 2;
			float z = -x + atan2f(-vMat[1].x, -vMat[2].x);
			vNewRot = Vector3{ x, y, z };
		}
		else { //two solutions exist
			float y1 = -asinf(vMat[0].z);
			float y2 = XM_PI - y1;

			float x1 = atan2f(vMat[1].z / cosf(y1), vMat[2].z / cosf(y1));
			float x2 = atan2f(vMat[1].z / cosf(y2), vMat[2].z / cosf(y2));

			float z1 = atan2f(vMat[0].y / cosf(y1), vMat[0].x / cosf(y1));
			float z2 = atan2f(vMat[0].y / cosf(y2), vMat[0].x / cosf(y2));

			//choose one solution to return
			//for example the "shortest" rotation
			if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
			{
				vNewRot = Vector3{ x1, y1, z1 };
			}
			else {
				vNewRot = Vector3{ x2, y2, z2 };
			}
		}
		return vNewRot;
	}

	bool closeEnough(const float& a, const float& b, const float& epsilon)
	{
		return (epsilon > std::abs(a - b));
	}

	void SetRandIndex(std::wstring& string, UINT range)
	{
		std::wstring idx = std::to_wstring(math::GetRandomNumber(1, range));
		string += idx;
	}

}
