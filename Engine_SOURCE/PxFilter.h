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

// 여기서 콜백에 관한 필터링을 할 수 있다.

static PxFilterFlags PlayerFilter (
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize
)
{
	// 트리거와 트리거 또는 트리거와 충돌하는 물체를 구분하여 처리합니다.
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		// 디버그해봐야함
		//if ((filterData1.word1 & filterData0.word0) || (filterData0.word1 & filterData1.word0))
		//{
		//	// 트리거와 충돌하는 물체 모두 OnTrigger 이벤트를 처리하도록 합니다.
		//	pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		//	return PxFilterFlag::eDEFAULT;
		//}

		// 트리거와 충돌하는 물체 모두 OnTrigger 이벤트를 처리하도록 합니다.
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	// 충돌하는 물체의 경우 충돌 플래그만 생성합니다.
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	return PxFilterFlag::eDEFAULT;

	// 디버그해봐야함
	// 두 필터가 서로 충돌플래그가 세워져 있을 경우
	/*if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlag::eKILL;*/
}



