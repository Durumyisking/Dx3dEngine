#include "PxFilter.h"

PxFilter::PxFilter()
{
}

PxFilter::~PxFilter()
{
}

PxQueryHitType::Enum PxFilter::preFilter(const PxFilterData& filterData, const PxShape* pShape, const PxRigidActor* pActor, PxHitFlags& queryFlags)
{

	// ��Ʈ �߻� �� ó��
	MessageBox(NULL, L"", L"", MB_OK);

	return PxQueryHitType::eTOUCH;
}

PxQueryHitType::Enum PxFilter::postFilter(const PxFilterData& filterData, const PxQueryHit& queryHit)
{
	return PxQueryHitType::eBLOCK;
}
