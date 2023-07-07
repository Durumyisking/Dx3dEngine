#include "Engine.h"
#include "PxEventCallback.h"


void PxEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			MessageBox(NULL, L"", L"", MB_OK);
		}
	}
}

void PxEventCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void PxEventCallback::onWake(PxActor** actors, PxU32 count)
{
}

void PxEventCallback::onSleep(PxActor** actors, PxU32 count)
{
}

void PxEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void PxEventCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}


