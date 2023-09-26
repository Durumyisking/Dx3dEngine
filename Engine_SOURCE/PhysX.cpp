#include "PhysX.h"
#include "PhysicsScene.h"
#include "ChracterController.h"
#include "CustomPhysXMemory.h"

PhysX::PhysX()
	:mPhysicsScene(nullptr)
	, mInitialization(nullptr)
	, mTransport (nullptr)
	, mSceneClient (nullptr)
	, mScene(nullptr)
	, mControllerMgr(nullptr)

{
	mInitialization = std::make_shared<PxInitialization>();
}
PhysX::~PhysX()
{
	if (mControllerMgr)
		mControllerMgr->release();

	if (mScene)
		mScene->release();
}

void PhysX::Init()
{
	mInitialization->CreateFoundation();
	mInitialization->CreateCooking();
	mInitialization->CreateVisualDebugger();
	mInitialization->ConntectVisualDebugger();
	mInitialization->CreatePhysics();
//	CreateSceneQuery();
}

PxConvexMesh* PhysX::CreateConvexMesh(const PxVec3* verts, const PxU32 numVerts, PxPhysics* physics, PxCooking* cooking)
{
	// Create descriptor for convex mesh
	physx::PxConvexMeshDesc convexDesc;
	convexDesc.points.count = numVerts;
	convexDesc.points.stride = sizeof(physx::PxVec3);
	convexDesc.points.data = verts;
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eSHIFT_VERTICES;

	physx::PxConvexMesh* convexMesh = NULL;
	physx::PxDefaultMemoryOutputStream buf;
	if (cooking->cookConvexMesh(convexDesc, buf))
	{
		physx::PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
		convexMesh = physics->createConvexMesh(id);
	}

	return convexMesh;
}

PxTriangleMesh* PhysX::CreateTriangleMesh(const PxVec3* verts, const PxU32 numVerts
	, const PxU32* indexs, const PxU32 numIndexes, PxPhysics* physics, PxCooking* cooking)
{
	// Create descriptor for triangle mesh
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = numVerts;
	meshDesc.points.stride = sizeof(PxVec3);	
	meshDesc.points.data = verts;

	meshDesc.triangles.count = numIndexes / 3;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = indexs;

	//// for prevent stackoverflow
	//PxU32 estimatedVertSize = numVerts * sizeof(PxVec3) * 1.5;
	//PxU32 estimatedIndexSize = numIndexes * sizeof(PxU32) * 1.5;
	//PxU32 initialSize = estimatedVertSize + estimatedIndexSize;
	//CustomPhysXMemory writeBuffer(initialSize);

	PxDefaultMemoryOutputStream writeBuffer;
	bool status = cooking->cookTriangleMesh(meshDesc, writeBuffer);
	if (!status)
		return nullptr; 

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxTriangleMesh* triangleMesh = physics->createTriangleMesh(readBuffer);

	return triangleMesh;
}

void PhysX::CreatePhysicsScene(const PxSceneDesc& sceneDesc)
{
	assert(mInitialization->GetPhysics());
	CreateScene(sceneDesc);
	//CreateControllerManager();

	mPhysicsScene = std::make_shared<PhysicsScene>(mScene);
}

void PhysX::CreateScene(const PxSceneDesc& sceneDesc)
{
	mScene = mInitialization->GetPhysics()->createScene(sceneDesc);
	assert(mScene);
}

void PhysX::CreateControllerManager()
{
	mControllerMgr = PxCreateControllerManager(*mScene);
	assert(mControllerMgr);
}

void PhysX::ConnectDebuggerToScene()
{
	mSceneClient = mScene->getScenePvdClient();
	assert(mSceneClient);

	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

}

void PhysX::CreateCharacterController(ChracterController* controller, PxMaterial* material)
{
	PxCapsuleControllerDesc desc = {};
	desc.height = controller->GetHeight();
	desc.radius = controller->GetRadius();
	desc.stepOffset = 0.3f;
	desc.volumeGrowth = 1.9f;
	desc.slopeLimit = cosf(XMConvertToRadians(15.f));
	desc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	desc.contactOffset = 0.5f;
	desc.upDirection = PxVec3(0.f, 1.f, 0.f);
	desc.material = material;

	desc.position = PxExtendedVec3(0.f, 0.f, 0.f);

	PxController* newController = mControllerMgr->createController(desc);
	assert(controller);

	controller->SetController(newController);
}

