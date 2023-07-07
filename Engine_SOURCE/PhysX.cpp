#include "PhysX.h"


PhysX::PhysX()
	:mPhysicsScene(nullptr)
	, mInitialization(nullptr)
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
	mInitialization->CreatePhysics();

//	CreateSceneQuery();
}

void PhysX::CreatePhysicsScene(const PxSceneDesc& SceneDesc)
{
	assert(mInitialization->GetPhysics());
	CreateScene(SceneDesc);
	CreateControllerManager();

	mPhysicsScene = std::make_shared<PhysicsScene>(mScene);
}

void PhysX::CreateScene(const PxSceneDesc& SceneDesc)
{
	mScene = mInitialization->GetPhysics()->createScene(SceneDesc);
	assert(mScene);
}

void PhysX::CreateControllerManager()
{
	mControllerMgr = PxCreateControllerManager(*mScene);
	assert(mControllerMgr);
}

