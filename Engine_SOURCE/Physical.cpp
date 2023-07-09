#include "Physical.h"
#include "GameObj.h"
#include "Transform.h"
#include "PhysicsMgr.h"
namespace dru
{
	Physical::Physical()
		: Component(eComponentType::Physical)
		, mActorType(eActorType::END)
		, mGeometryType(eGeometryType::END)
		, mSize(dru::math::Vector3::Zero)
		, mActor(nullptr)
		, mShape(nullptr)
		, mController(nullptr)
		, mProperties(nullptr)
		, mGeometries(nullptr)
		, mTransform(nullptr)
	{
	}

	Physical::~Physical()
	{
	}

	void Physical::Initialize()
	{
		mTransform = GetOwner()->GetComponent<dru::Transform>();
		CreateGeometry(mGeometryType, mSize);
		CreateActor();

		if (mActor)
		{
			AddActor(mActor);
		}
	}

	void Physical::InitialPhysics(eActorType actorType, eGeometryType geometryType, dru::math::Vector3 geometrySize, MassProperties massProperties)
	{
		mActorType = actorType;
		mGeometryType = geometryType;
		mSize = geometrySize;

		CreatePhysicsProperties();
	}

	void Physical::update()
	{
		if (mTransform->IsScaleChanged())
		{
			Vector3 scale = mTransform->GetScale();

			if (eActorType::CHARACTER == mActorType)
			{
				PxBoxController* pController = static_cast<PxBoxController*>(mController);
				pController->setHalfSideExtent(pController->getHalfSideExtent() * scale.x);
				pController->setHalfHeight(pController->getHalfHeight() * scale.y);
				pController->setHalfForwardExtent(pController->getHalfForwardExtent() * scale.z);
			}

			else
			{
				switch (mGeometryType)
				{
				case eGeometryType::BOX:
				{
					mGeometries->boxGeom.halfExtents = dru::convert::Vector3ToPxVec3(mSize * scale);
					ApplyShapeScale();
				}
				break;
				case eGeometryType::CAPSULE:
				{
					mGeometries->capsuleGeom.radius = mSize.x * scale.x;
					mGeometries->capsuleGeom.radius = mSize.y * scale.y;
					ApplyShapeScale();
				}
				break;
				}
			}

			mTransform->ScaleChangedOff();
		}
	}

	void Physical::fixedUpdate()
	{
	}

	void Physical::render()
	{
	}

	void Physical::CreateBoxGeometry(eGeometryType geometryType, dru::math::Vector3 boxSize)
	{
		assert(eGeometryType::BOX == geometryType);
		assert(nullptr == mGeometries);
		mGeometries = std::make_shared<Geometries>(geometryType, boxSize);
	}

	void Physical::CreateCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight)
	{
		assert(eGeometryType::CAPSULE == geometryType);
		assert(nullptr == mGeometries);
		mGeometries = std::make_shared<Geometries>(geometryType, radius, halfHeight);
	}

	void Physical::CreatePlaneGeometry(eGeometryType geometryType)
	{
		assert(eGeometryType::PLANE == geometryType);
		assert(nullptr == mGeometries);
		mGeometries = std::make_shared<Geometries>(geometryType);
	}

	void Physical::CreatePhysicsProperties(MassProperties massProperties)
	{
		mProperties = std::make_shared<PhysicalProperties>(massProperties);
	}

	void Physical::CreateGeometry(eGeometryType geometryType, dru::math::Vector3 shapeSize)
	{
		mSize = shapeSize;
		mSize *= mTransform->GetScale();

		switch (geometryType)
		{
		case eGeometryType::BOX:
			CreateBoxGeometry(geometryType, shapeSize);
			break;
		case eGeometryType::CAPSULE:
			CreateCapsuleGeometry(geometryType, shapeSize.x, shapeSize.y);
			break;
		case eGeometryType::PLANE:
			CreatePlaneGeometry(geometryType);
			break;
		}
	}

	void Physical::CreateShape()
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

	void Physical::CreateActor()
	{
		PxPhysics* physics = PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysics();

		if (physics)
		{
			switch (mActorType)
			{
			case eActorType::DYNAMIC:
				mActor = physics->createRigidDynamic(PxTransform(dru::convert::Vector3ToPxVec3(mTransform->GetPosition())));
				break;
			case eActorType::STATIC:
				mActor = physics->createRigidStatic(PxTransform(dru::convert::Vector3ToPxVec3(mTransform->GetPosition())));
				break;
			case eActorType::KINEMATIC:
			{
				mActor = physics->createRigidDynamic(PxTransform(dru::convert::Vector3ToPxVec3(mTransform->GetPosition())));
				PxRigidDynamic* pActor = mActor->is<PxRigidDynamic>();
				pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			}
			break;

			case eActorType::CHARACTER:
			{
				CreateController();

				if (mController)
				{
					//mController->setUserData();
				}
			}
			break;
			}

			if (mActor)
			{
				InitializeActor();
			}
		}
	}

	void Physical::InitializeActor()
	{
		CreateShape();

		// Collider에서 세팅
		PxFilterData filterData = {};
		filterData.word0 = 1 << 1;
		filterData.word1 = 1 << 0;
		mShape->setSimulationFilterData(filterData);

		switch (mActorType)
		{
		case eActorType::KINEMATIC:
		case eActorType::DYNAMIC:
		{
			PxRigidDynamic* pActor = mActor->is<PxRigidDynamic>();
			pActor->attachShape(*mShape);
			pActor->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z);
		}
		break;

		case eActorType::STATIC:
		{
			PxRigidStatic* pActor = mActor->is<PxRigidStatic>();
			pActor->attachShape(*mShape);
		}
		break;
		}

		PxRigidActor* pActor = mActor->is<PxRigidActor>();
		pActor->userData = PhysicsMgr::GetInstance()->GetDispatcher()->GetSimulationCallback();
	}

	void Physical::CreateController()
	{
		switch (mGeometryType)
		{
		case eGeometryType::BOX:
		{
			PxBoxControllerDesc desc;
			desc.setToDefault();
			// Half Size이므로 2를 곱해서 크기 복원
			desc.halfSideExtent = mSize.x;
			desc.halfHeight = mSize.y;
			desc.halfForwardExtent = mSize.z;
			desc.material = mProperties->GetMaterial();
			mController = static_cast<PxBoxController*>(PhysicsMgr::GetInstance()->GetEnvironment()->GetControllerManager()->createController(desc));
		}
		break;
		case eGeometryType::CAPSULE:
		{
			PxCapsuleControllerDesc desc;
			desc.setToDefault();
			desc.radius = mSize.x;
			desc.height = mSize.y;
			desc.material = mProperties->GetMaterial();
			mController = static_cast<PxCapsuleController*>(PhysicsMgr::GetInstance()->GetEnvironment()->GetControllerManager()->createController(desc));
		}
		break;
		}

		mController->setPosition(dru::convert::Vector3ToPxExtendedVec3(mTransform->GetPosition()));
	}

	void Physical::AddActor(PxActor* actor)
	{
		PhysicsMgr::GetInstance()->GetEnvironment()->GetPhysScene()->AddActor(actor);
	}

	void Physical::ApplyShapeScale()
	{
		switch (mActorType)
		{
		case eActorType::STATIC:
		{
			PxRigidStatic* pActor = mActor->is<PxRigidStatic>();
			pActor->detachShape(*mShape);
			mShape->release();

			CreateShape();
			pActor->attachShape(*mShape);
		}
		break;
		case eActorType::DYNAMIC:
		case eActorType::KINEMATIC:
		{
			PxRigidDynamic* pActor = mActor->is<PxRigidDynamic>();
			pActor->detachShape(*mShape);
			mShape->release();

			CreateShape();
			pActor->attachShape(*mShape);
		}
		break;
		}
	}

}