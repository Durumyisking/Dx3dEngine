#include "Physical.h"
#include "GameObj.h"
#include "PhysicsMgr.h"
#include "PhysXRigidBody.h"

namespace dru
{
	Physical::Physical()
		: Component(eComponentType::Physical)
		, mActorType(eActorType::END)
		, mGeometryType(eGeometryType::END)
		, mSize(dru::math::Vector3::Zero)
		, mActor(nullptr)
		, mShape(nullptr)
		, mProperties(nullptr)
		, mGeometries(nullptr)
	{
	}

	Physical::~Physical()
	{
	}

	void Physical::Initialize()
	{
		assert(GetOwner()->GetComponent<PhysXRigidBody>());
		initializeActor();
	}

	void Physical::InitialPhysics(eActorType actorType, eGeometryType geometryType, dru::math::Vector3 geometrySize, MassProperties massProperties)
	{
		mActorType = actorType;
		mGeometryType = geometryType;
		mSize = geometrySize;

		createPhysicsProperties(massProperties);
		createGeometry(mGeometryType, mSize);
		createActor();
		createUniversalShape();
	}

	void Physical::update()
	{

	}

	void Physical::fixedUpdate()
	{
	}

	void Physical::render()
	{
	}


	void Physical::AddActorToPxScene()
	{
		GETSINGLE(PhysicsMgr)->GetInstance()->GetEnvironment()->GetPhysScene()->AddActor(mActor);
	}

	void Physical::RemoveActorToPxScene()
	{
		GETSINGLE(PhysicsMgr)->GetInstance()->GetEnvironment()->GetPhysScene()->RemoveActor(mActor);
	}

	void Physical::createBoxGeometry(eGeometryType geometryType, const Vector3& boxSize)
	{
		assert(eGeometryType::BOX == geometryType);
		assert(nullptr == mGeometries);
		mGeometries = std::make_shared<Geometries>(geometryType, boxSize);
	}

	void Physical::createCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight)
	{
		assert(eGeometryType::CAPSULE == geometryType);
		assert(nullptr == mGeometries);
		mGeometries = std::make_shared<Geometries>(geometryType, radius, halfHeight);
	}

	void Physical::createPlaneGeometry(eGeometryType geometryType)
	{
		assert(eGeometryType::PLANE == geometryType);
		assert(nullptr == mGeometries);
		mGeometries = std::make_shared<Geometries>(geometryType);
	}

	void Physical::createSphereGeometry(eGeometryType geometryType, float fRadius)
	{
		assert(eGeometryType::SPHERE == geometryType);
		assert(nullptr == mGeometries);
		mGeometries = std::make_shared<Geometries>(geometryType);
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
		case eGeometryType::BOX:
			createBoxGeometry(geometryType, mSize);
			break;

		case eGeometryType::CAPSULE:
			createCapsuleGeometry(geometryType, mSize.x, mSize.y);
			break;

		case eGeometryType::SPHERE:
			createSphereGeometry(geometryType, mSize.x);
			break;

		case eGeometryType::PLANE:
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
			case eGeometryType::BOX:
				mShape = physics->createShape(mGeometries->boxGeom, *mProperties->GetMaterial());
				break;
			case eGeometryType::CAPSULE:
				mShape = physics->createShape(mGeometries->capsuleGeom, *mProperties->GetMaterial());
				break;
			case eGeometryType::PLANE:
				mShape = physics->createShape(mGeometries->planeGeom, *mProperties->GetMaterial());
				break;
			}
		}
	}

	void Physical::createShape()
	{
		switch (mGeometryType)
		{
		case eGeometryType::BOX:
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometries->boxGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::CAPSULE:
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometries->capsuleGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::SPHERE:
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometries->sphereGeom, *mProperties->GetMaterial());
			break;
		case eGeometryType::PLANE:
			mShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(), mGeometries->planeGeom, *mProperties->GetMaterial());
			break;
		}
	}

	void Physical::createActor()
	{
		PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();

		if (physics)
		{
			switch (mActorType)
			{
			case eActorType::DYNAMIC:
				mActor = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
				break;
			case eActorType::STATIC:
				mActor = physics->createRigidStatic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
				break;
			case eActorType::KINEMATIC:
			{
				mActor = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
				mActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			}
			break;

			case eActorType::CHARACTER:
			{
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
		case eActorType::STATIC:
			break;
		//case eActorType::MONSTER_DYNAMIC:
		//case eActorType::PROJECTILE_DYNAMIC:
		case eActorType::DYNAMIC:
			//PhysXRigidBody rigidBody = GetOwner()->GetComponent<PhysXRigidBody>();
			//rigidBody->SetLinearDamping(0.5f);
			//rigidBody->SetLinearMaxVelocityForDynamic(1000.f);
			//rigidBody->SetAngularMaxVelocityForDynamic(500.f);
			break;
		case eActorType::KINEMATIC:
			mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
			break;
		}
	}
}