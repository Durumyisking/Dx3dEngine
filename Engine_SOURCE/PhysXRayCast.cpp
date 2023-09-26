#include "PhysXRayCast.h"
#include "Physical.h"
#include "SceneMgr.h"
#include "Layer.h"
#include "Scene.h"
#include "Renderer.h"
#include "Camera.h"
#include "InputMgr.h"
#include "Transform.h"
#include "Application.h"

extern Application application;

PhysXRayCast::PhysXRayCast()
	: mRaycastHit{}
	, mRayMaxDist(6500.f)
	, mRayMaxHit(10)
	, mPickingObject(nullptr)
	, mPickDistance(FLT_MAX)
{

	CollisionLayerCheck(eLayerType::Player);
	CollisionLayerCheck(eLayerType::Boss);
	CollisionLayerCheck(eLayerType::Objects);
	CollisionLayerCheck(eLayerType::PhysicalObject);
	CollisionLayerCheck(eLayerType::Monster);
}

PhysXRayCast::~PhysXRayCast()
{
}

GameObj* PhysXRayCast::Raycast()
{
	Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();

	// Get the cursor position
	math::Vector2 clientPos = GETSINGLE(InputMgr)->GetMousePosition_client();

	float windowX = static_cast<float>(application.GetWidth());
	float windowY = static_cast<float>(application.GetHeight());

	float ndcX = (2.0f * clientPos.x / windowX) - 1.0f;
	float ndcY = 1.0f - (2.0f * clientPos.y / windowY);

	// Create the NDC position with z = --1.0
	Vector4 ndcPos = { ndcX, ndcY, 0.f, 1.0f };
	//DirectX::XMFLOAT4 ndcPos(ndcX, ndcY, 0.f, 1.0f);
	//DirectX::XMFLOAT4 ndcPos2(ndcX, ndcY, 1.f, 1.0f);

	// Unproject NDC position to world space
	const Matrix& invProj = renderer::mainCamera->GetProjectionMatrix().Invert();
	const Matrix& invView = renderer::mainCamera->GetViewMatrix().Invert();

	ndcPos = Vector4::Transform(ndcPos, invProj);
	ndcPos = Vector4::Transform(ndcPos, invView);

	const Vector3& RayOrigin = renderer::mainCamera->GetOwnerPos();
	Vector3 RayDirection = Vector3(ndcPos.x - RayOrigin.x, ndcPos.y - RayOrigin.y, ndcPos.z - RayOrigin.z);
	RayDirection.Normalize();

	for (size_t i = 0; i < static_cast<UINT>(enums::eLayerType::End); i++)
	{
		if (!mLayerCollision[i])
			continue;

		Layer& layer = scene->GetLayer((enums::eLayerType)i);
		const std::vector<GameObj*>& gameObjs
			= layer.GetGameObjects();

		for (GameObj* obj : gameObjs)
		{
			if (!CollisionCheck(RayOrigin, RayDirection, obj, mRayMaxDist))
				continue;

			if (mPickDistance > static_cast<float>(mRaycastHit.distance))
			{
				mPickingObject = obj;
				mPickDistance = static_cast<float>(mRaycastHit.distance);
			}
		}
	}

	return mPickingObject;
}

void PhysXRayCast::ReleaseRaycast()
{
	mPickingObject = nullptr;
	mPickDistance = FLT_MAX;
}

bool PhysXRayCast::MoveObject()
{
	if (mPickingObject == nullptr)
		return false;

	math::Vector2 clientPos = GETSINGLE(InputMgr)->GetMousePosition_client();
	math::Vector2 prevClientPos = GETSINGLE(InputMgr)->GetPrevMousePosition_client();
	
	math::Vector2 ratio = application.GetResolutionRatio();

	math::Vector2 move = clientPos - prevClientPos;
	move.x /= ratio.x;
	move.y /= ratio.y;

	
	Vector3 Up = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->Up();
	Vector3 Forward = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->Forward();
	Vector3 Right = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->Right();

	math::Vector3 objPos = mPickingObject->GetComponent<Transform>()->GetPhysicalPosition();

	if (abs(move.x) > abs(move.y))
	{
		objPos += Right * move.x * 0.1f;
	}
	else
	{
		objPos += Up * -move.y * 0.1f;
	}
	mPickingObject->GetComponent<Transform>()->SetPhysicalPosition(objPos);
	
	return true;
}

bool PhysXRayCast::MoveObject(GameObj* obj)
{
	return false;
}

bool PhysXRayCast::CollisionCheck(const Vector3& origin, const Vector3& dir, GameObj* gameObject, float maxDistance)
{
	Physical* physical = gameObject->GetComponent<Physical>();
	if (!physical)
	{
		return false;
	}
	eGeometryType geometryType = physical->GetGeometryType();
	PxTransform pxTransform = gameObject->GetComponent<Transform>()->GetPxTransform();

	if (geometryType == eGeometryType::ConvexMesh || geometryType == eGeometryType::TriangleMesh)
		int a = 0;

	switch (geometryType)
	{
	case eGeometryType::Box:
	{
		PxBoxGeometry boxGeom = physical->GetGeometries()->boxGeom;
		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			boxGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		return bResult;
	}
	break;

	case eGeometryType::Capsule:
	{
		PxCapsuleGeometry capsuleGeom = physical->GetGeometries()->capsuleGeom;
		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			capsuleGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		return bResult;
	}
	break;

	case eGeometryType::Sphere:
	{
		PxSphereGeometry sphereGeom = physical->GetGeometries()->sphereGeom;

		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			sphereGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		if (bResult)
			bResult = true;

		return bResult;
	}
	case eGeometryType::ConvexMesh:
	{
		PxTriangleMeshGeometry triangleGeom = physical->GetGeometries()->triangleMeshGeom;

		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			triangleGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		if (bResult)
			bResult = true;

		return bResult;
	}
		break;
	case eGeometryType::TriangleMesh:
	{
		PxConvexMeshGeometry convexGeom = physical->GetGeometries()->convexMeshGeom;

		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			convexGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		if (bResult)
			bResult = true;

		return bResult;
	}
		break;
	break;
	}

	return false;
}

