#pragma once
#include "Engine.h"



using namespace math;
class PhysXCollisionMgr
{
	SINGLE(PhysXCollisionMgr)

public:
	void SetCollisionGroup(eLayerType first, eLayerType second);
	void Update();

	void SetForceInLayer(eLayerType layerType, const Vector3& pos, const Vector3& volume, const Vector3& impulse);

private:
	void setCollisionFlag(UINT first, UINT second);


private:
	std::bitset<static_cast<UINT>(eLayerType::End)> mLayerCollisionMatrix[static_cast<UINT>(eLayerType::End)];

};
