#pragma once
#include "Engine.h"

class PxFilter 
	: public PxQueryFilterCallback
{
public:
	PxFilter();
	~PxFilter();

	virtual PxQueryHitType::Enum preFilter(
		const PxFilterData& filterData, const PxShape* pShape,
		const PxRigidActor* pActor, PxHitFlags& queryFlags);

	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& queryHit);

private:

};

// =======================
// PlayerSimulationShaders
// =======================

// ���⼭ �ݹ鿡 ���� ���͸��� �� �� �ִ�.

static PxFilterFlags PlayerFilter (
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize
)
{
	// Ʈ���ſ� �浹�ϴ� ��ü ��� OnTrigger �̺�Ʈ �� �浹 �̺�Ʈ�� ó���ϵ��� ����
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		// Ʈ���ſ� �浹�ϴ� ��ü ��� OnTrigger �̺�Ʈ�� ó���ϵ��� �մϴ�.
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}
	else if (PxFilterObjectIsKinematic(attributes0) || PxFilterObjectIsKinematic(attributes1))
	{
		int i = 0;
	}
	// �� �� Ʈ���Ű� �ƴ� ��� �浹 �÷��� ����
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	return PxFilterFlag::eDEFAULT;

	//// �� ���Ͱ� ���� �浹�÷����� ���
	//if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
	//{
	//	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
	//	return PxFilterFlag::eDEFAULT;
	//}

	//return PxFilterFlag::eKILL;
}



