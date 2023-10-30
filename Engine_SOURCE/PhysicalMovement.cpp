#include "PhysicalMovement.h"
#include "GameObj.h"
#include "Transform.h"
#include "TimeMgr.h"
#include "PhysXRigidBody.h"
#include "Physical.h"


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
	if (eLayerType::Player== GetOwner()->GetLayerType())
	{
	}
	GetOwner()->ReorganizePosition(AXIS::Y, eLayerType::Platforms, true);
	GetOwner()->ReorganizePosition(AXIS::XZ, eLayerType::Objects);

	Vector3 velocity = GetOwner()->GetComponent<PhysXRigidBody>()->GetVelocity();

	Move(velocity);
}
void PhysicalMovement::Render()
{
}


void PhysicalMovement::Move(const Vector3& velocity)
{
	PxTransform transform = GetOwner()->GetComponent<Transform>()->GetPxTransform();
	transform.p += convert::Vector3ToPxVec3(velocity * DT);
		
	Physical* physical = GetOwner()->GetComponent<Physical>();

	if (eActorType::Kinematic == physical->GetActorType())
	{
		if(!physical->IsKinematicActorSleep())
			physical->GetActor<PxRigidDynamic>()->setKinematicTarget(transform);
	}
	else
	{
		physical->GetActor<PxRigidDynamic>()->setGlobalPose(transform);
	}

}
