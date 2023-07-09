#include "PhysXRigidBody.h"
#include "GameObj.h"
#include "Physical.h"

namespace dru
{
	PhysXRigidBody::PhysXRigidBody()
		: Component(eComponentType::RigidBody)
		, mActorType(eActorType::CHARACTER)
	{
	}
	PhysXRigidBody::~PhysXRigidBody()
	{
	}
	void PhysXRigidBody::Initialize()
	{
	}
	void PhysXRigidBody::update()
	{
	}
	void PhysXRigidBody::fixedUpdate()
	{
		// 계산결과를 가져온다.
		switch (mActorType)
		{
		case eActorType::DYNAMIC:
		{
			PxTransform transform = static_cast<PxRigidDynamic*>(GetOwner()->GetComponent<Physical>()->GetActor())->getGlobalPose();
			GetOwner()->SetPos(convert::PxVec3ToVector3(transform.p));
		}
		break;
		case eActorType::STATIC:
			break;
		case eActorType::KINEMATIC:
			break;
		case eActorType::CHARACTER:
			GetOwner()->SetPos(convert::PxExtendedVec3ToVector3(GetOwner()->GetComponent<Physical>()->GetController()->getPosition()));
			break;
		}
	}
	void PhysXRigidBody::render()
	{
	}
}
