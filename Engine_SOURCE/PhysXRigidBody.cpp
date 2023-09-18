#include "PhysXRigidBody.h"
#include "GameObj.h"
#include "Physical.h"
#include "TimeMgr.h"
#include "Transform.h"


using namespace math;
PhysXRigidBody::PhysXRigidBody()
	: Component(eComponentType::RigidBody)
	, mPhysical(nullptr)
	, mGravityApplied(true)
	, mFriction(Vector3(20.f, 0.0f, 20.0f))
	, mForce(Vector3::Zero)
	, mGravityAccel(Vector3(0.f, -9.8f * 4.f, 0.f))
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
	// 이동
	//  F = M x A
	//  A = F / M

	// 가속도 계산
	mAccelation = mForce / mMass;

	// 속도에 가속도를더함
	mVelocity += mAccelation * DT;

	if (mGravityApplied) // 공중에 있을때 중력 영향 0
	{
		mVelocity += mGravityAccel * DT;
	}
	else				// 공중 X
	{

		// 가속도, 중력 정규화
		Vector3 gravity = mGravityAccel;
		gravity.Normalize();

		Vector3 velocity = mVelocity;
		velocity.Normalize();

		// 내적
		float dot = velocity.Dot(gravity);

		mVelocity -= gravity * dot;
	}

	//마찰력 조건 (적용된 힘이 없고, 속도가 0 이 아닐때)
	if (( mVelocity.x != 0.f || mVelocity.z != 0.f ))
	{
		// 속도 반대 방향으로 마찰력을 적용
		math::Vector3 friction = -mVelocity;
		friction.Normalize();

		friction = friction * mFriction * mMass * DT;

		// 마찰력으로 인한 속도 감소량 현재 속도보다 더 큰 경우
		Vector3 sideVelo = mVelocity;
		sideVelo.y = 0.f;

		float mVelocityLength = sideVelo.Length();
		float frictionLength = friction.Length();
		if (mVelocityLength < frictionLength)
		{
			// 속도를 0 으로 만든다
			float yForce = mVelocity.y;
			mVelocity = Vector3::Zero;
			mVelocity.y = yForce;
		}
		else
		{
			// 속도에서 마찰략으로 인한 반대방향으로 속도를 차감
			mVelocity += friction;
		}
	}

	// 이동
	Vector3 Pos = mOwnerTransform->GetPhysicalPosition();
	Pos += mVelocity * DT;
	mOwnerTransform->SetPhysicalPosition(Pos);

	// 힘 초기화
	mForce = Vector3::Zero;
	// TEST
	return;

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

