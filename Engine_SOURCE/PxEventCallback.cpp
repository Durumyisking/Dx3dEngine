#include "Engine.h"
#include "PxEventCallback.h"
#include "GameObj.h"
#include "PhysXCollider.h"


void PxEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
            {
                GameObj* contact = static_cast<GameObj*>(pairHeader.actors[0]->userData);
                GameObj* other = static_cast<GameObj*>(pairHeader.actors[1]->userData);

                if (contact && other)
                {
                    PhysXCollider* contactColl = contact->GetComponent<PhysXCollider>();
                    PhysXCollider* otherColl = other->GetComponent<PhysXCollider>();

                    if (contactColl)
                        contactColl->OnCollisionEnter(otherColl);

                    if (otherColl)
                        otherColl->OnCollisionEnter(contactColl);
                }
            }
        }
        // on collision �κ� : ontrigger�� �����ϸ� �Ǵ°� �´ٰ� �� (���� �浹ü������ �ε����� �� �����ӿ� ���� �о����)
        //else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        //{
        //    if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
        //    {
        //        GameObj* contact = static_cast<GameObj*>(pairHeader.actors[0]->userData);
        //        GameObj* other = static_cast<GameObj*>(pairHeader.actors[1]->userData);

        //        if (contact && other)
        //        {
        //            PhysXCollider* contactColl = contact->GetComponent<PhysXCollider>();
        //            PhysXCollider* otherColl = other->GetComponent<PhysXCollider>();

        //            if (contactColl)
        //                contactColl->OnCollisionEnter(otherColl);

        //            if (otherColl)
        //                otherColl->OnCollisionEnter(contactColl);
        //        }
        //    }
        //}

        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
            {
                GameObj* contact = static_cast<GameObj*>(pairHeader.actors[0]->userData);
                GameObj* other = static_cast<GameObj*>(pairHeader.actors[1]->userData);

                PhysXCollider* contactColl = contact->GetComponent<PhysXCollider>();
                PhysXCollider* otherColl = other->GetComponent<PhysXCollider>();

                if (contactColl)
                    contactColl->OnCollisionExit(otherColl);

                if (otherColl)
                    otherColl->OnCollisionExit(contactColl);
            }
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
    for (PxU32 i = 0; i < count; i++)
    {
        const PxTriggerPair& pair = pairs[i];

        if (pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            if (pair.triggerActor && pair.otherActor)
            {
                GameObj* trigger = static_cast<GameObj*>(pair.triggerActor->userData);
                GameObj* other = static_cast<GameObj*>(pair.otherActor->userData);

                if (trigger && other)
                {
                    PhysXCollider* triggerColl = trigger->GetComponent<PhysXCollider>();
                    PhysXCollider* otherColl = other->GetComponent<PhysXCollider>();

                    if (triggerColl)
                        triggerColl->OnTriggerEnter(otherColl);

                    // ������ �ֹ����� trigger ȣ���ϱ⶧���� �� �� �����ʿ� �����
                    //if (otherColl)
                    //    otherColl->OnTriggerEnter(triggerColl);
                }
            }
        }

        if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            if (pair.triggerActor && pair.otherActor)
            {
                GameObj* trigger = static_cast<GameObj*>(pair.triggerActor->userData);
                GameObj* other = static_cast<GameObj*>(pair.otherActor->userData);

                if (trigger && other)
                {
                    PhysXCollider* triggerColl = trigger->GetComponent<PhysXCollider>();
                    PhysXCollider* otherColl = other->GetComponent<PhysXCollider>();

                    if (triggerColl)
                        triggerColl->OnTriggerExit(otherColl);

                    // ������ �ֹ����� trigger ȣ���ϱ⶧���� �� �� �����ʿ� �����
                    //if (otherColl)
                    //    otherColl->OnTriggerExit(triggerColl);
                }
            }
        }
    }
}

void PxEventCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
    int i = 0;
}


