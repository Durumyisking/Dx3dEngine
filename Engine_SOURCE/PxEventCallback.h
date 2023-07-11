#pragma once
#include "Engine.h"


// scene 쿼리는 콜백으로 client에게 도달하지 않는다

class PxEventCallback
	: public physx::PxSimulationEventCallback
{
public:
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void onWake(PxActor** actors, PxU32 count) override;
	virtual void onSleep(PxActor** actors, PxU32 count) override;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;
};

/*
onContact():
이 메서드는 물체 간의 접촉이 발생할 때 호출됩니다.

이 메서드를 사용하여 물체 간의 충돌, 접촉, 마찰 등에 대한 정보를 받을 수 있습니다.
이를 통해 추가적인 충돌 검출, 충돌 응답, 데이터 수집 등을 수행할 수 있습니다.

onConstraintBreak():
이 메서드는 제약 조건이 깨지는 경우 호출됩니다.

예를 들어, 물리적 제약 조건(물체 간의 연결, 조인트 등)이 불안정해지거나 파괴될 때 발생합니다.
이 콜백을 사용하여 제약 조건의 깨짐에 대한 피드백을 받거나 추가 동작을 수행할 수 있습니다.

onWake():
이 메서드는 물체가 깨어날 때 호출됩니다.

물리 시뮬레이션에서는 물체를 비활성화하여 계산 부하를 줄이는 경우가 있습니다.
이 메서드를 사용하여 물체가 다시 활성화될 때 알림을 받고, 필요한 작업을 수행할 수 있습니다.

onSleep():
이 메서드는 물체가 잠들 때 호출됩니다.

물리 시뮬레이션에서는 물체가 일정 시간 동안 정지되면 비활성화하여 계산 부하를 줄일 수 있습니다.
이 메서드를 사용하여 물체가 잠들었을 때 알림을 받고, 필요한 작업을 수행할 수 있습니다.


onTrigger():
이 메서드는 트리거(충돌 검출만 수행되고 실제 충돌 응답은 발생하지 않는 영역)가 활성화되거나 비활성화될 때 호출됩니다. 

트리거 영역을 사용하여 특정 조건을 충족하는 물체 간의 상호작용을 처리할 수 있습니다.
이 메서드를 사용하여 트리거 이벤트에 대한 알림을 받고, 필요한 작업을 수행할 수 있습니다.


*/

