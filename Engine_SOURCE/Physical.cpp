#include "Physical.h"
#include "GameObj.h"
#include "PhysicsMgr.h"
#include "PhysXRigidBody.h"
#include "PhysicsScene.h"
#include "MeshRenderer.h"
#include "Renderer.h"



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
	assert(GetOwner()->GetComponent<PhysXRigidBody>());
	initializeActor();
}

void Physical::InitialDefaultProperties(eActorType actorType, eGeometryType geometryType, math::Vector3 geometrySize, MassProperties massProperties)
{
	if (geometryType == eGeometryType::ConvexMesh)
	{
		InitialConvexMeshProperties(actorType, geometrySize, massProperties);
		return;
	}

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

void Physical::InitialConvexMeshProperties(eActorType actorType, Vector3 geometrySize, MassProperties massProperties)
{
	PxConvexMesh* convexMesh = MakeConvexObject();

	if (convexMesh == nullptr)
		assert(false);

	mActorType = actorType;
	mGeometryType = eGeometryType::ConvexMesh;
	mSize = geometrySize;
	createPhysicsProperties(massProperties);
	createConvexMeshGeometry(mGeometryType, convexMesh, mSize);
	createActor();
	createConvexShape(convexMesh);
	AddActorToPxScene();
	
}

PxConvexMesh* Physical::MakeConvexObject()
{

	std::shared_ptr<PhysX>physX = GETSINGLE(PhysicsMgr)->GetEnvironment();

	Model* model = GetOwner()->GetComponent<MeshRenderer>()->GetModel();
	if (model == nullptr)
		return nullptr;

	//vertexCount = model->GetNumberOfVertices(index);
	const std::vector<Mesh*> meshes = model->GetMeshes();
	std::vector<std::vector<renderer::Vertex>> meshVertices = model->GetVertexes();
	PxU32 vertexCount = 0;
	std::vector<PxVec3> vertices;

	for (Mesh* mesh : meshes)
	{
		std::vector<renderer::Vertex> meshVertices = mesh->GetVertexes();
		PxU32 count = mesh->GetVertexCount();
		vertexCount += count;

		// Copy from cvector array to PxVec3 array
		for (PxU32 i = 0; i < count; i++)
		{
			vertices.push_back(PxVec3(meshVertices[i].pos.x, meshVertices[i].pos.y, meshVertices[i].pos.z));
		}
	}

	PxVec3* v = vertices.data();

	return physX->CreateConvexMesh(v, vertexCount, physX->GetPhysics(), physX->GetCooking());
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
	case enums::eGeometryType::ConvexMesh:
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

void Physical::createConvexMeshGeometry(eGeometryType geometryType, PxConvexMesh* convexMesh, const Vector3& mScale)
{
	assert(eGeometryType::ConvexMesh == geometryType);
	assert(nullptr == mGeometry);
	mGeometry = std::make_shared<Geometry>(geometryType, convexMesh, mScale);
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
		�� �ڵ��

		PxRigidActorExt::createExclusiveShape() �̰Ͱ� ����.
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

void Physical::createConvexShape(PxConvexMeshGeometry convexMesh)
{
	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*mActor->is<PxRigidActor>(),
		mGeometry->convexMeshGeom, *mProperties->GetMaterial());
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
		rigidBody->SetLinearMaxVelocityForDynamic(1000.f);
		rigidBody->SetAngularMaxVelocityForDynamic(500.f);
	}
		break;
	case eActorType::Kinematic:
		/*
			eSIMULATION_SHAPE 
			�ش� �÷��׸� Ű�� Kinematic Actor�� Shape�� �����ùķ��̼ǿ� �����մϴ�.
		
			eSCENE_QUERY_SHAPE
			�ش� �÷��׸� Ű�� Kinematic Shape�� ����ĳ��Ʈ, ��ŷ���� �۾����� �浹�� Ȯ���� �� �ֽ��ϴ�.

			eTRIGGER_SHAPE
			�ش� �÷��׸� Ű�� Kinematic Shape�� Ʈ���ŷ� ���˴ϴ�. Ʈ���Ÿ� ���ؼ� �浹 �̺�Ʈ�� ���� �� �� �ְ� �˴ϴ�.

			eVISUALIZATION
			�ش� �÷��׸� Ű�� Kinematic ��ü�� �ð�ȭ �������� ���˴ϴ�.
		*/


		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		break;
	}
}
