#include "PhysX.h"


PhysX::PhysX()
	:mPhysicsScene(nullptr)
	, mInitialization(nullptr)
	, mTransport (nullptr)
	, mPvd (nullptr)
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

	if (mPvd)
		mPvd->release();

	if (mTransport)
		mTransport->release();
}

void PhysX::Init()
{
	mInitialization->CreateFoundation();
	mInitialization->CreatePhysics();

//	CreateSceneQuery();
}

void PhysX::CreatePhysicsScene(const PxSceneDesc& sceneDesc)
{
	assert(mInitialization->GetPhysics());
	CreateScene(sceneDesc);
	CreateControllerManager();

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

void PhysX::CreateDebugger(const char* szHost, __int32 iPort)
{
}

void PhysX::ConnectDebugger()
{
}

