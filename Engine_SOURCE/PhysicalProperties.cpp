#include "PhysicalProperties.h"
#include "Engine.h"
#include "PhysicsMgr.h"

PhysicalProperties::PhysicalProperties(const MassProperties& massProperties)
	: mStaticFriction(massProperties.mStaticFriction)
	, mDynamicFriction(massProperties.mDynamicFriction)
	, mRestitution(massProperties.mRestitution)
	, mMaterial(nullptr)
{
	// �������� �Ӽ����� ������ ��Ƽ���� ����
	mMaterial = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics()->createMaterial(mStaticFriction, mDynamicFriction, mRestitution);
}

PhysicalProperties::~PhysicalProperties()
{
}

void PhysicalProperties::ApplyToShape(PxShape* pShape)
{
	if (mMaterial)
	{
		mMaterial->setStaticFriction(mStaticFriction);
		mMaterial->setDynamicFriction(mDynamicFriction);
		mMaterial->setRestitution(mRestitution);

		pShape->setMaterials(&mMaterial, 1);
	}
}

void PhysicalProperties::SetStaticFriction(float fFriction)
{
	mStaticFriction = fFriction;
	if (mMaterial)
		mMaterial->setStaticFriction(mStaticFriction);
}

void PhysicalProperties::SetDynamicFriction(float fFriction)
{
	mDynamicFriction = fFriction;
	if (mMaterial)
		mMaterial->setDynamicFriction(mDynamicFriction);
}

void PhysicalProperties::SetRestitution(float fRestitution)
{
	mRestitution = fRestitution;
	if (mMaterial)
		mMaterial->setRestitution(mRestitution);
}
