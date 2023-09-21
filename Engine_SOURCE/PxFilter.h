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
	// Ʈ���ſ� Ʈ���� �Ǵ� Ʈ���ſ� �浹�ϴ� ��ü�� �����Ͽ� ó���մϴ�.
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		// ������غ�����
		//if ((filterData1.word1 & filterData0.word0) || (filterData0.word1 & filterData1.word0))
		//{
		//	// Ʈ���ſ� �浹�ϴ� ��ü ��� OnTrigger �̺�Ʈ�� ó���ϵ��� �մϴ�.
		//	pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		//	return PxFilterFlag::eDEFAULT;
		//}

		// Ʈ���ſ� �浹�ϴ� ��ü ��� OnTrigger �̺�Ʈ�� ó���ϵ��� �մϴ�.
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	// �浹�ϴ� ��ü�� ��� �浹 �÷��׸� �����մϴ�.
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	return PxFilterFlag::eDEFAULT;

	// ������غ�����
	// �� ���Ͱ� ���� �浹�÷��װ� ������ ���� ���
	/*if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlag::eKILL;*/
}



