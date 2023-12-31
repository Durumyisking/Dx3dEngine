#pragma once
#include "Engine.h"
#include "PxInitialization.h"

class PhysicsScene;
class ChracterController;
class PhysX
{
public:
	PhysX();
	~PhysX();

public:
	void Init();

public:
	PxConvexMesh* CreateConvexMesh(const PxVec3* verts, const PxU32 numVerts, PxPhysics* physics, PxCooking* cooking);
	PxTriangleMesh* CreateTriangleMesh(const PxVec3* verts, const PxU32 numVerts
		, const PxU32* indexes, const PxU32 numIndexes, PxPhysics* physics, PxCooking* cooking);

	PxCooking*				GetCooking() const { return mInitialization->GetCooking(); }
	PxFoundation*			GetFoundation() const { return mInitialization->GetFoundation(); }
	PxPhysics*				GetPhysics() const { return mInitialization->GetPhysics(); }
	PxControllerManager*	GetControllerManager() const { return mControllerMgr; }

	std::shared_ptr<PhysicsScene>	GetPhysicsScene() const { return mPhysicsScene; }

	void CreatePhysicsScene(const PxSceneDesc& sceneDesc);
	void ConnectDebuggerToScene();

	void CreateCharacterController(ChracterController* controller, PxMaterial* material);

private:
	void CreateScene(const PxSceneDesc& sceneDesc);
	void CreateControllerManager();


private:
	std::shared_ptr<PhysicsScene>		mPhysicsScene;
	std::shared_ptr<PxInitialization>	mInitialization;


	PxPvdTransport*			mTransport;
	PxPvdSceneClient*		mSceneClient;
	PxScene*				mScene;
	PxControllerManager*	mControllerMgr;
};

