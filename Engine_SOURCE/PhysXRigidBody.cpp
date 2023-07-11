#include "PhysXRigidBody.h"
#include "GameObj.h"
#include "Physical.h"
#include "TimeMgr.h"

namespace dru
{
	using namespace math;
	PhysXRigidBody::PhysXRigidBody()
		: Component(eComponentType::RigidBody)
		, mPhysical(nullptr)
		, mGravityApplied(true)
		, mGravityAccel(Vector3(0.f, -4500.f, 0.f))
		, mMaxVelocity( Vector3(1000.f, -1500.f, 1.f))
		, mReserveTimer(0.f)
	{
	}

	PhysXRigidBody::~PhysXRigidBody()
	{
	}

	void PhysXRigidBody::Initialize()
	{
		mPhysical = GetOwner()->GetComponent<Physical>();
	}

	void PhysXRigidBody::update()
	{
	}

	void PhysXRigidBody::fixedUpdate()
	{
		if (mGravityApplied)
		{
			if (mVelocity.y > mMaxVelocity.y)
				mVelocity += mGravityAccel * DT;
		}

		if (mReserveTimer.IsRunning())
		{
			mReserveTimer.Update(DT);
			mVelocity = mReserveVelocity;

			if (mReserveTimer.IsFinished())
			{
				mReserveTimer.Stop();
				mReserveVelocity = Vector3::Zero;
				mVelocity = Vector3::Zero;
			}
		}
	}

	void PhysXRigidBody::render()
	{
	}

	// for kinematic actors
	float PhysXRigidBody::GetVelocity(AXIS eAxis)
	{
		switch (eAxis)
		{
		case AXIS::X:
			return mVelocity.x;
		case AXIS::Y:
			return mVelocity.y;
		case AXIS::Z:
			return mVelocity.z;
		}

		assert(nullptr);
		return PX_MAX_F32;
	}

	// for kinematic actors
	void PhysXRigidBody::SetVelocity(AXIS eAxis, float velocity)
	{
		switch (eAxis)
		{
		case AXIS::X:
			mVelocity.x = velocity;
			break;
		case AXIS::Y:
			mVelocity.y = velocity;
			break;
		}
	}
	// for kinematic actors
	void PhysXRigidBody::AddVelocity(AXIS eAxis, float velocity)
	{
		switch (eAxis)
		{
		case AXIS::X:
			mVelocity.x += velocity;
			break;
		case AXIS::Y:
			mVelocity.y += velocity;
			break;
		}
	}

	void PhysXRigidBody::ReserveSpeedForSeconds(const Vector3& velocity, float duration)
	{
		mReserveVelocity = velocity;
		mReserveTimer.SetEndTime(duration);
		mReserveTimer.Start();
	}

	// for dynamic actors
	void PhysXRigidBody::SetMassForDynamic(float mass)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setMass(mass);
	}
	// for dynamic actors
	void PhysXRigidBody::SetLinearVelocityForDynamic(const PxVec3& linearVelocity)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setLinearVelocity(linearVelocity);
	}

	void PhysXRigidBody::SetLinearVelocityForDynamic(AXIS eAxis, float valVelocity)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		PxVec3 velocity = dynamicActor->getLinearVelocity();

		switch (eAxis)
		{
		case AXIS::X:
			dynamicActor->setLinearVelocity(PxVec3(valVelocity, velocity.y, velocity.z));
			break;
		case AXIS::Y:
			dynamicActor->setLinearVelocity(PxVec3(velocity.x, valVelocity, velocity.z));
			break;
		case AXIS::Z:
			dynamicActor->setLinearVelocity(PxVec3(velocity.x, velocity.y, valVelocity));
			break;
		}
	}

	// for dynamic actors
	void PhysXRigidBody::SetAngularVelocityForDynamic(const PxVec3& angularVelocity)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setAngularVelocity(angularVelocity);
	}

	// for dynamic actors
	void PhysXRigidBody::AddForceForDynamic(const PxVec3& force, PxForceMode::Enum eForceMode)
	{
		assert(mPhysical);
		assert(mPhysical->GetActor<PxRigidDynamic>());
		PxRigidBodyExt::addForceAtPos(
			*mPhysical->GetActor<PxRigidDynamic>(),
			force,
			convert::Vector3ToPxVec3(GetOwner()->GetComponent<Transform>()->GetPhysicalPosition()),
			eForceMode);
	}

	// for dynamic actors
	void PhysXRigidBody::SetLinearDamping(float damping)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setLinearDamping(damping);
	}

	// for dynamic actors
	void PhysXRigidBody::SetAngularDamping(float damping)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setAngularDamping(damping);
	}

	// for dynamic actors
	void PhysXRigidBody::SetLinearMaxVelocityForDynamic(float maxVelocity)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setMaxLinearVelocity(maxVelocity);
	}

	// for dynamic actors
	void PhysXRigidBody::SetAngularMaxVelocityForDynamic(float maxVelocity)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setMaxAngularVelocity(maxVelocity);
	}

	// for dynamic actors
	void PhysXRigidBody::ApplyGravityForDynamic()
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	}

	// for dynamic actors
	void PhysXRigidBody::RemoveGravityForDynamic()
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);
		dynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	}

	void PhysXRigidBody::SetRotationZForDynamic(float rotationZ)
	{
		assert(mPhysical);
		PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
		assert(dynamicActor);

		auto transform = dynamicActor->getGlobalPose();
		transform.q.z = rotationZ;
		dynamicActor->setGlobalPose(transform);
	}

}
