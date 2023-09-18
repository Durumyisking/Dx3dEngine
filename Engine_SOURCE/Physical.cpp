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
	, mShape(nullptr)
	, mProperties(nullptr)
	, mGeometry(nullptr)
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
	createGeometry(mGeometryType, mSize);
	createActor();
	createShape();
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
		mGeometry->boxGeom.halfExtents = convert::Vector3ToPxVec3(newSize * 0.5f);
		mShape->setGeometry(mGeometry->boxGeom);
		break;
	case enums::eGeometryType::Capsule:
		mGeometry->capsuleGeom.halfHeight = newSize.y * 0.5f;
		mGeometry->capsuleGeom.radius = newSize.x * 0.5f;
		mShape->setGeometry(mGeometry->capsuleGeom);
		break;
	case enums::eGeometryType::Sphere:
		mGeometry->sphereGeom.radius = newSize.x * 0.5f;
		mShape->setGeometry(mGeometry->sphereGeom);
		break;
	case enums::eGeometryType::Plane:
		break;
	case enums::eGeometryType::End:
		break;
	default:
		break;
	}
}

void Physical::createBoxGeometry(eGeometryType geometryType, const Vector3& boxSize)
{
	assert(eGeometryType::Box == geometryType);
	assert(nullptr == mGeometry);
	mGeometry = std::make_shared<Geometry>(geometryType, boxSize);
}

void Physical::createCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight)
{
	assert(eGeometryType::Capsule == geometryType);
	assert(nullptr == mGeometry);
	mGeometry = std::make_shared<Geometry>(geometryType, radius, halfHeight);
}

void Physical::createPlaneGeometry(eGeometryType geometryType)
{
	assert(eGeometryType::Plane == geometryType);
	assert(nullptr == mGeometry);
	mGeometry = std::make_shared<Geometry>(geometryType);
}

void Physical::createSphereGeometry(eGeometryType geometryType, float radius)
{
	assert(eGeometryType::Sphere == geometryType);
	assert(nullptr == mGeometry);
	mGeometry = std::make_shared<Geometry>(geometryType, radius);
}

void Physical::createPhysicsProperties(const MassProperties& massProperties)
{
	mProperties = std::make_shared<PhysicalProperties>(massProperties);
}

void Physical::createGeometry(eGeometryType geometryType, const Vector3& shapeSize)
{
	mSize = shapeSize;

	switch (geometryType)
	{
	case eGeometryType::Box:
		createBoxGeometry(geometryType, mSize);
		break;

	case eGeometryType::Capsule:
		createCapsuleGeometry(geometryType, mSize.x, mSize.y);
		break;

	case eGeometryType::Sphere:
		createSphereGeometry(geometryType, mSize.x);
		break;

	case eGeometryType::Plane:
		createPlaneGeometry(geometryType);
		break;

	default:
		assert(true);
		break;
	}
}

void Physical::createUniversalShape()
{
	PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();

	if (physics)
	{
		switch (mGeometryType)
		{
		case eGeometryType::Box:
			mShape = physics->createShape(mGeometry->boxGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Capsule:
			mShape = physics->createShape(mGeometry->capsuleGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Plane:
			mShape = physics->createShape(mGeometry->planeGeom, *mProperties->GetMaterial());
			break;
		}
	}
}

void Physical::createShape()
{
	/*
		PxShape* shape = physics.createShape(PxSphereGeometry(1.0f), myMaterial, true);
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
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometry->boxGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Capsule:	
		{
			PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)));
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometry->capsuleGeom, *mProperties->GetMaterial());
			mShape->setLocalPose(relativePose);
		}
			break;
		case eGeometryType::Sphere:
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometry->sphereGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::Plane:
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometry->planeGeom, *mProperties->GetMaterial());
			break;
		}
	}
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
			mActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
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
		break;
	//case eActorType::MONSTER_DYNAMIC:
	//case eActorType::PROJECTILE_DYNAMIC:
	case eActorType::Dynamic:
	{
		PhysXRigidBody* rigidBody = GetOwner()->GetComponent<PhysXRigidBody>();
		rigidBody->SetLinearDamping(0.5f);
		rigidBody->SetLinearMaxVelocityForDynamic(100.f);
		rigidBody->SetAngularMaxVelocityForDynamic(50.f);
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


		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		break;
	}
}
