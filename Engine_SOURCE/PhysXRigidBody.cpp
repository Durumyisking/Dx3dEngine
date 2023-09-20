#include "PhysXRigidBody.h"
#include "GameObj.h"
#include "Physical.h"
#include "TimeMgr.h"
#include "Transform.h"


using namespace math;
PhysXRigidBody::PhysXRigidBody()
	: Component(eComponentType::RigidBody)
	, mPhysical(nullptr)
	, mbGravityApplied(true)
	, mbAirborn(true)
	, mFriction(Vector3(20.f, 0.0f, 20.0f))
	, mFricCoeff(40.f)
	, mForce(Vector3::Zero)
	, mGravityAccel(Vector3(0.f, -9.8f * 4.f	 , 0.f))
	, mMaxVelocity(Vector3(100.f, 200.f, 100.f))
	, mReserveTimer(0.f)
	, mAccelation(Vector3(0.f, 0.f, 0.f))
	, mMass(1.0f)
	, mOwnerTransform(nullptr)
{
}

PhysXRigidBody::~PhysXRigidBody()
{
}

void PhysXRigidBody::Initialize()
{
	mPhysical = GetOwner()->GetComponent<Physical>();

	if (eActorType::Dynamic == mPhysical->GetActorType())
	{
		SetAngularMaxVelocityForDynamic(40.f);
		SetLinearMaxVelocityForDynamic(40.f);
	}

	mOwnerTransform = GetOwner()->GetComponent<Transform>();
	assert(mOwnerTransform);
}

void PhysXRigidBody::Update()
{
}

void PhysXRigidBody::FixedUpdate()
{
	// accel from force
	float fForce = mForce.Length();

	if (0.f != fForce)
	{
		mForce.Normalize();

		float Accel = fForce / mMass;

		mAccelation = mForce * Accel;
	}

	if (mbAirborn && mbGravityApplied)
	{
		mAccelation += mGravityAccel;
	}

	mVelocity += mAccelation * DT;


	// cal fric
	if (mVelocity != Vector3::Zero && !mbAirborn)
	{
		Vector3 FricDir = -mVelocity;
		FricDir.Normalize();

		Vector3 Friction = FricDir * mFricCoeff * DT;

		mAccelation += Friction;

		if (mVelocity.Length() <= Friction.Length())
		{
			mVelocity = Vector3(0.f, 0.f, 0.f);
		}
		else
		{
			mVelocity += Friction;
		}
	}

	// check max speed
	if (mMaxVelocity.x < fabs(mVelocity.x))
	{
		mVelocity.x /= fabs(mVelocity.x);
		mVelocity.x *= mMaxVelocity.x;
	}
	if (mMaxVelocity.z < fabs(mVelocity.z))
	{
		mVelocity.z /= fabs(mVelocity.z);
		mVelocity.z *= mMaxVelocity.z;
	}
	if (mMaxVelocity.y < fabs(mVelocity.y))
	{
		mVelocity.y /= fabs(mVelocity.y);
		mVelocity.y *= mMaxVelocity.y;
	}

	mForce = Vector3(0.f, 0.f, 0.f);
	mAccelation = Vector3(0.f, 0.f, 0.f);
}

void PhysXRigidBody::Render()
{
}

void PhysXRigidBody::SetVelocity(AXIS axis, const math::Vector3& velocity)
{
	switch (axis)
	{
	case enums::AXIS::X:
		mVelocity.x = velocity.x;
		break;
	case enums::AXIS::Y:
		mVelocity.y = velocity.y;
		break;
	case enums::AXIS::Z:
		mVelocity.z = velocity.z;
		break;
	case enums::AXIS::XY:
		mVelocity.x = velocity.x;
		mVelocity.y = velocity.y;
		break;
	case enums::AXIS::XZ:
		mVelocity.x = velocity.x;
		mVelocity.z = velocity.z;
		break;
	case enums::AXIS::YZ:
		mVelocity.y = velocity.y;
		mVelocity.z = velocity.z;
		break;
	case enums::AXIS::XYZ:
		mVelocity.x = velocity.x;
		mVelocity.y = velocity.y;
		mVelocity.z = velocity.z;
		break;
	case enums::AXIS::END:
		break;
	default:
		break;
	}

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


// for dynamic actors
void PhysXRigidBody::SetMassForDynamic(float mass)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->setMass(mass);
}
// for dynamic actors
void PhysXRigidBody::SetLinearVelocityForDynamic(const math::Vector3& linearVelocity)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->setLinearVelocity(convert::Vector3ToPxVec3(linearVelocity));
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
void PhysXRigidBody::SetAngularVelocityForDynamic(const math::Vector3& angularVelocity)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->setAngularVelocity(convert::Vector3ToPxVec3(angularVelocity));
}

// for dynamic actors
void PhysXRigidBody::AddForceForDynamic(const math::Vector3& force, PxForceMode::Enum eForceMode)
{
	assert(mPhysical);
	assert(mPhysical->GetActor<PxRigidDynamic>());
	PxRigidBodyExt::addForceAtPos(
		*mPhysical->GetActor<PxRigidDynamic>(),
		convert::Vector3ToPxVec3(force),
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

void PhysXRigidBody::SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum flag, bool value)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->setRigidDynamicLockFlag(flag, value);
}

void PhysXRigidBody::AddTorqueForDynamic(const math::Vector3& torque)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->addTorque(convert::Vector3ToPxVec3(torque), PxForceMode::Enum::eFORCE);
}

void PhysXRigidBody::AddTorqueXForDynamic(const float& torque)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->addTorque(convert::Vector3ToPxVec3(Vector3(torque, 0.f, 0.f)), PxForceMode::Enum::eFORCE);
}

void PhysXRigidBody::AddTorqueYForDynamic(const float& torque)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->addTorque(convert::Vector3ToPxVec3(Vector3(0.f, torque, 0.f)), PxForceMode::Enum::eFORCE);
}

void PhysXRigidBody::AddTorqueZForDynamic(const float& torque)
{
	assert(mPhysical);
	PxRigidDynamic* dynamicActor = mPhysical->GetActor<PxRigidDynamic>();
	assert(dynamicActor);
	dynamicActor->addTorque(convert::Vector3ToPxVec3(Vector3(0.f, 0.f, torque)), PxForceMode::Enum::eFORCE);
}

