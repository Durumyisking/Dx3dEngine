#include "Physical.h"
#include "GameObj.h"
#include "PhysicsMgr.h"
#include "PhysXRigidBody.h"
#include "PhysicsScene.h"



Physical::Physical()
	: Component(eComponentType::Physical)
	, mActorType(eActorType::End)
	, mGeometryType(eGeometryType::End)
	, mSize(math::Vector3::Zero)
	, mActor(nullptr)
	, mMainShape(nullptr)
	, mSubShapes{}
	, mProperties(nullptr)
	, mMainGeometry(nullptr)
	, mSubGeometries{}
{
}

Physical::~Physical()
{
}

void Physical::Initialize()
{
	//assert(GetOwner()->GetComponent<PhysXRigidBody>());
	initializeActor();
}

void Physical::InitialDefaultProperties(eActorType actorType, eGeometryType geometryType, math::Vector3 geometrySize, MassProperties massProperties)
{
	mActorType = actorType;
	mGeometryType = geometryType;
	mSize = geometrySize;

	createPhysicsProperties(massProperties);
	mMainGeometry = std::make_shared<Geometry>(createGeometry(mGeometryType, mSize));
	createActor();
	CreateMainShape();
	AddActorToPxScene();
//		createUniversalShape();
}

void Physical::Update()
{

}

void Physical::FixedUpdate()
{
}

void Physical::Render()
{
	
}


void Physical::AddActorToPxScene()
{
	GETSINGLE(PhysicsMgr)->GetInstance()->GetEnvironment()->GetPhysicsScene()->AddActor(mActor);
}

void Physical::RemoveActorToPxScene()
{
	GETSINGLE(PhysicsMgr)->GetInstance()->GetEnvironment()->GetPhysicsScene()->RemoveActor(mActor);
}

void Physical::SetGeometrySize(const Vector3& newSize)
{
	switch (mGeometryType)
	{
	case enums::eGeometryType::Box:
		mMainGeometry->boxGeom.halfExtents = convert::Vector3ToPxVec3(newSize * 0.5f);
		mMainShape->setGeometry(mMainGeometry->boxGeom);
		break;
	case enums::eGeometryType::Capsule:
		mMainGeometry->capsuleGeom.halfHeight = newSize.y * 0.5f;
		mMainGeometry->capsuleGeom.radius = newSize.x * 0.5f;
		mMainShape->setGeometry(mMainGeometry->capsuleGeom);
		break;
	case enums::eGeometryType::Sphere:
		mMainGeometry->sphereGeom.radius = newSize.x * 0.5f;
		mMainShape->setGeometry(mMainGeometry->sphereGeom);
		break;
	case enums::eGeometryType::Plane:
		break;
	case enums::eGeometryType::End:
		break;
	default:
		break;
	}
}

Geometry Physical::createBoxGeometry(eGeometryType geometryType, const Vector3& boxSize)
{
	assert(eGeometryType::Box == geometryType);
	return Geometry(geometryType, boxSize);
}

Geometry Physical::createCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight)
{
	assert(eGeometryType::Capsule == geometryType);
	return Geometry(geometryType, radius, halfHeight);
}

Geometry Physical::createPlaneGeometry(eGeometryType geometryType)
{
	assert(eGeometryType::Plane == geometryType);
	return Geometry(geometryType);
}

Geometry Physical::createSphereGeometry(eGeometryType geometryType, float radius)
{
	assert(eGeometryType::Sphere == geometryType);
	return Geometry(geometryType, radius);
}

void Physical::createPhysicsProperties(const MassProperties& massProperties)
{
	mProperties = std::make_shared<PhysicalProperties>(massProperties);
}

Geometry Physical::createGeometry(eGeometryType geometryType, const Vector3& shapeSize)
{
	//mSize = shapeSize;

	switch (geometryType)
	{
	case eGeometryType::Box:
		return createBoxGeometry(geometryType, shapeSize);
		break;

	case eGeometryType::Capsule:
		return createCapsuleGeometry(geometryType, shapeSize.x, shapeSize.y);
		break;

	case eGeometryType::Sphere:
		return createSphereGeometry(geometryType, shapeSize.x);
		break;

	case eGeometryType::Plane:
		return createPlaneGeometry(geometryType);
		break;

	default:
		assert(true);
		break;
	}
	assert(true);
	Geometry geom = {};
	return geom;
}

void Physical::createUniversalShape()
{
	PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();

	if (physics)
	{
		switch (mGeometryType)
		{
		case eGeometryType::Box:
			mMainShape = physics->createShape(mMainGeometry->boxGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Capsule:
			mMainShape = physics->createShape(mMainGeometry->capsuleGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Plane:
			mMainShape = physics->createShape(mMainGeometry->planeGeom, *mProperties->GetMaterial());
			break;
		}
	}
}

void Physical::CreateMainShape()
{
	/*
		PxShape* shape = physics.CreateShape(PxSphereGeometry(1.0f), myMaterial, true);
		myActor.attachShape(*shape);
		shape->release();
		이 코드는

		PxRigidActorExt::createExclusiveShape() 이것과 같다.
	*/
	PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();

	if (physics)
	{
		switch (mGeometryType)
		{
		case eGeometryType::Box:
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->boxGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Capsule:	
		{
			PxTransform tr(PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)));
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->capsuleGeom, *mProperties->GetMaterial());
			mMainShape->setLocalPose(tr);
		}
			break;
		case eGeometryType::Sphere:
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->sphereGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Plane:
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->planeGeom, *mProperties->GetMaterial());
			break;
		}
	}
}

void Physical::CreateMainShape(Vector3 localPos)
{
	PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();

	if (physics)
	{
		PxTransform tr = {};
		tr.p = convert::Vector3ToPxVec3(localPos);
		switch (mGeometryType)
		{
		case eGeometryType::Box:
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->boxGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Capsule:
		{
			tr.q = (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)));
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->capsuleGeom, *mProperties->GetMaterial());
		}
		break;
		case eGeometryType::Sphere:
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->sphereGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Plane:
			mMainShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mMainGeometry->planeGeom, *mProperties->GetMaterial());
			break;
		}
		mMainShape->setLocalPose(tr);
	}
}

void Physical::CreateSubShape(Vector3 relativePos, eGeometryType geomType, Vector3 geomSize, PxShapeFlag::Enum shapeFlag)
{
	PxShape* shape = nullptr;
	
	PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();
	if (physics && mMainShape)
	{
		UINT idx = static_cast<UINT>(mSubShapes.size());

		mSubGeometries.push_back(std::make_shared<Geometry>(createGeometry(geomType, geomSize)));

		PxTransform relativePose = GetActor<PxRigidDynamic>()->getGlobalPose();
		relativePose.p = relativePose.p + convert::Vector3ToPxVec3(relativePos);
		switch (geomType)
		{
		case eGeometryType::Box:
			shape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mSubGeometries[idx]->boxGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Capsule:
		{
			relativePose.q = (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)));
			shape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mSubGeometries[idx]->capsuleGeom, *mProperties->GetMaterial());
		}
		break;
		case eGeometryType::Sphere:
			shape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mSubGeometries[idx]->sphereGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Plane:
			shape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mSubGeometries[idx]->planeGeom, *mProperties->GetMaterial());
			break;
		}
		if (shape)
		{
			shape->setLocalPose(relativePose);

			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

			mSubShapes.push_back(shape);
		}

	}
	//shape->release();
}

void Physical::createActor()
{
	PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();

	if (physics)
	{
		switch (mActorType)
		{
		case eActorType::Dynamic:
			mActor = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
			//mActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			break;
		case eActorType::Static:
			mActor = physics->createRigidStatic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
			break;
		case eActorType::Kinematic:
		{
			mActor = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
			//mActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			mActor->is<PxRigidDynamic>()->setRigidBodyFlags(PxRigidBodyFlag::eKINEMATIC);
		}
		break;
		case eActorType::Character:
		{
		}
		break;
		case eActorType::Monster:
		{
			mActor = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
			mActor->is<PxRigidDynamic>()->setRigidDynamicLockFlags(
				PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
				PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
			break;
		}
		break;

		}
	}
}

void Physical::initializeActor()
{
	PxRigidActor* actor = mActor->is<PxRigidActor>();
	actor->userData = GetOwner();

	PxVec3 pos = convert::Vector3ToPxVec3(GetOwnerPos());
	actor->setGlobalPose(PxTransform(pos));

	switch (mActorType)
	{
	case eActorType::Static:
		mMainShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
		mMainShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		break;
	//case eActorType::MONSTER_DYNAMIC:
	//case eActorType::PROJECTILE_DYNAMIC:
	case eActorType::Dynamic:
	{
		PhysXRigidBody* rigidBody = GetOwner()->GetComponent<PhysXRigidBody>();
		rigidBody->SetLinearDamping(0.5f);
		rigidBody->SetLinearMaxVelocityForDynamic(100.f);
		rigidBody->SetAngularMaxVelocityForDynamic(50.f);
		mMainShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
		mMainShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	}
		break;
	case eActorType::Kinematic:
		/*
			eSIMULATION_SHAPE 
			해당 플래그를 키면 Kinematic Actor의 Shape가 물리시뮬레이션에 참여합니다.
		
			eSCENE_QUERY_SHAPE
			해당 플래그를 키면 Kinematic Shape가 레이캐스트, 픽킹등의 작업에서 충돌을 확인할 수 있습니다.

			eTRIGGER_SHAPE
			해당 플래그를 키면 Kinematic Shape가 트리거로 사용됩니다. 트리거를 통해서 충돌 이벤트를 설정 할 수 있게 됩니다.

			eVISUALIZATION
			해당 플래그를 키면 Kinematic 객체가 시각화 목적으로 사용됩니다.
		*/

		mMainShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		mMainShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		break;
	}
}
