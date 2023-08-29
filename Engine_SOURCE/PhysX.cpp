#include "PhysX.h"
#include "PhysicsScene.h"


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
	mInitialization->CreateVisualDebugger();
	mInitialization->ConntectVisualDebugger();
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

void PhysX::ConnectDebuggerToScene()
{
	mSceneClient = mScene->getScenePvdClient();
	assert(mSceneClient);

	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

}

