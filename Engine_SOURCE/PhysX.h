#pragma once
#include "Engine.h"
#include "PxInitialization.h"

class PhysicsScene;
class PhysX
{
public:
	PhysX();
	~PhysX();

public:
	void Init();

public:
	PxFoundation*			GetFoundation() const { return mInitialization->GetFoundation(); }
	PxPhysics*				GetPhysics() const { return mInitialization->GetPhysics(); }
	PxControllerManager*	GetControllerManager() const { return mControllerMgr; }

	std::shared_ptr<PhysicsScene>	GetPhysScene() const { return mPhysicsScene; }

	void CreatePhysicsScene(const PxSceneDesc& sceneDesc);

private:
	void CreateScene(const PxSceneDesc& sceneDesc);
	void CreateControllerManager();
	void CreateDebugger(const char* szHost, __int32 iPort);
	void ConnectDebugger();

private:
	std::shared_ptr<PhysicsScene>		mPhysicsScene;
	std::shared_ptr<PxInitialization>	mInitialization;


	PxPvdTransport*			mTransport;
	PxPvd*					mPvd;
	PxPvdSceneClient*		mSceneClient;
	PxScene*				mScene;
	PxControllerManager*	mControllerMgr;
};

