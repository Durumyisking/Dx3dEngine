#pragma once
#include "EngineResource.h"
#include "Engine.h"
#include "SimpleMath.h"
#include "GameObj.h"
#include "PhysicalProperties.h"
#include "SimpleMath.h"

class PhysXRayCast
{
	SINGLE(PhysXRayCast);
public:

	void Raycast();
	void ReleaseRaycast();

	bool MoveObject();
	bool MoveObject(GameObj* obj);

	bool CollisionCheck(const math::Vector3& origin, const math::Vector3& dir, GameObj* gameObject, float maxDistance = 100.f);

	void CollisionLayerCheck(eLayerType type, bool enable = true) { mLayerCollision[static_cast<UINT>(type)] = enable; }


private:
	PxRaycastHit    mRaycastHit;
	float			mRayMaxDist;
	UINT			mRayMaxHit;

	GameObj* mPickingObject;
	float mPickDistance;
	std::bitset<static_cast<UINT>(eLayerType::End)> mLayerCollision;
};

