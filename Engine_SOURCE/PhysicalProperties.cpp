#include "PhysicalProperties.h"
#include "Engine.h"
#include "PhysicsMgr.h"

PhysicalProperties::PhysicalProperties(const MassProperties& massProperties)
	: mStaticFriction(massProperties.mStaticFriction)
	, mDynamicFriction(massProperties.mDynamicFriction)
	, mRestitution(massProperties.mRestitution)
	, mMaterial(nullptr)
{
	// 설정해준 속성으로 피직서 머티리얼 생성
	mMaterial = GETSINGLE(PhysicsMgr)->GetEnvironment()->GetPhysics()->createMaterial(mStaticFriction, mDynamicFriction, mRestitution);
}

PhysicalProperties::~PhysicalProperties()
{
}

void PhysicalProperties::ApplyToShape(PxShape& shape)
{
	if (mMaterial)
	{
		mMaterial->setStaticFriction(mStaticFriction);
		mMaterial->setDynamicFriction(mDynamicFriction);
		mMaterial->setRestitution(mRestitution);

		shape.setMaterials(&mMaterial, 1);
	}
}

void PhysicalProperties::SetStaticFriction(float friction)
{
	mStaticFriction = friction;
	if (mMaterial)
		mMaterial->setStaticFriction(mStaticFriction);
}

void PhysicalProperties::SetDynamicFriction(float friction)
{
	mDynamicFriction = friction;
	if (mMaterial)
		mMaterial->setDynamicFriction(mDynamicFriction);
}

void PhysicalProperties::SetRestitution(float restitution)
{
	mRestitution = restitution;
	if (mMaterial)
		mMaterial->setRestitution(mRestitution);
}
