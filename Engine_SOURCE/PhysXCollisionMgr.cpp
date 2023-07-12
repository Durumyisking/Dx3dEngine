#include "PhysXCollisionMgr.h"
#include "PhysXCollider.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "GameObj.h"
#include "PhysXCollider.h"

namespace dru
{
	PhysXCollisionMgr::PhysXCollisionMgr()
	{

	}

	PhysXCollisionMgr::~PhysXCollisionMgr()
	{

	}

	void PhysXCollisionMgr::SetCollisionGroup(eLayerType first, eLayerType second)
	{
		mLayerCollisionMatrix[static_cast<UINT>(first)][static_cast<UINT>(second)] = true;
	}

	void PhysXCollisionMgr::Update()
	{
		for (UINT i = 0; i < static_cast<UINT>(eLayerType::End); ++i)
		{
			for (UINT j = 0; j < static_cast<UINT>(eLayerType::End); ++j)
			{
				if (mLayerCollisionMatrix[i][j])
				{
					setCollisionFlag(i, j);
				}
			}
		}
	}

	void PhysXCollisionMgr::SetForceInLayer(eLayerType layerType, const Vector3& pos, const Vector3& volume, const Vector3& impulse)
	{
	}

	void PhysXCollisionMgr::SetForceInPlayerAndTakeDamage(const Vector3& pos, const Vector3& volume, const Vector3& impulse, float damage)
	{
	}

	void PhysXCollisionMgr::SetForceFromDotInLayerAndTakeDamage(eLayerType layerType, const Vector3& pos, const Vector3& volume, const Vector3& impulse, float damage)
	{
	}

	void PhysXCollisionMgr::SetForceInLayerAndFollowPlayer(eLayerType layerType, const Vector3& pos, const Vector3& volume, const Vector3& impulse)
	{
	}

	void PhysXCollisionMgr::setCollisionFlag(UINT first, UINT second)
	{
		std::vector<GameObj*> gameObjects = GETSINGLE(SceneMgr)->GetActiveScene()->GetLayer((static_cast<eLayerType>(first))).GetGameObjects();
		for (GameObj* gameObject : gameObjects)
		{
			if (gameObject->GetComponent<PhysXCollider>())
			{
				PxFilterData& filterData = gameObject->GetComponent<PhysXCollider>()->GetFilterData();
				if (filterData.word0 & (1 << first))
				{
					filterData.word1 |= (1 << second);
				}
			}
		}
	}

}