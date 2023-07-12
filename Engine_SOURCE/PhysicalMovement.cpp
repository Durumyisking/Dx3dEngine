#include "PhysicalMovement.h"
#include "GameObj.h"
#include "Transform.h"
#include "TimeMgr.h"
#include "PhysXRigidBody.h"
#include "Physical.h"

namespace dru
{
	PhysicalMovement::PhysicalMovement()
		:Component(eComponentType::Movement)
	{
	}
	PhysicalMovement::~PhysicalMovement()
	{
	}
	void PhysicalMovement::Initialize()
	{
	}
	void PhysicalMovement::Update()
	{
	}
	void PhysicalMovement::FixedUpdate()
	{
		Vector3 vVelocity = GetOwner()->GetComponent<PhysXRigidBody>()->GetVelocity();

		if (eLayerType::Player== GetOwner()->GetLayerType())
		{
		}

		Move(vVelocity);
	}
	void PhysicalMovement::Render()
	{
	}
	void PhysicalMovement::Move(const Vector3& velocity)
	{
		PxTransform transform = GetOwner()->GetComponent<Transform>()->GetPxTransform();
		transform.p += convert::Vector3ToPxVec3(velocity * DT);

		Physical* physical = GetOwner()->GetComponent<Physical>();

		if (eActorType::KINEMATIC == physical->GetActorType())
			physical->GetActor<PxRigidDynamic>()->setKinematicTarget(transform);
		else
			physical->GetActor<PxRigidDynamic>()->setGlobalPose(transform);

		//GetOwner()->GetComponent<Transform>()->SetPhysicalPosition(convert::PxVec3ToVector3(transform.p));
	}
}