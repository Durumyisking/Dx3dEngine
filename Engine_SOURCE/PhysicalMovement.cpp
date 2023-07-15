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
		Vector3 velocity = GetOwner()->GetComponent<PhysXRigidBody>()->GetVelocity();

		if (eLayerType::Player== GetOwner()->GetLayerType())
		{
		}

		Move(velocity);
	}
	void PhysicalMovement::Render()
	{
	}

	// 이동은 여기서 수행
	void PhysicalMovement::Move(const Vector3& velocity)
	{
		PxTransform transform = GetOwner()->GetComponent<Transform>()->GetPxTransform();
		transform.p += convert::Vector3ToPxVec3(velocity * DT);
		
		Physical* physical = GetOwner()->GetComponent<Physical>();

		if (eActorType::Kinematic == physical->GetActorType())
		{
			physical->GetActor<PxRigidDynamic>()->setKinematicTarget(transform);
		}

	}
}