#include "PhysicsMgr.h"

#include "PhysX.h"
#include "TimeMgr.h"
#include "PxEventCallback.h"
#include "Engine.h"
#include "PxFilter.h"


PhysicsMgr::PhysicsMgr()
	: mPhysX(nullptr)
	, mDispatcher(nullptr)
{
	mPhysX = std::make_shared<PhysX>();
	mDispatcher = std::make_shared<PxDispatcher>();
}

PhysicsMgr::~PhysicsMgr()
{
	//PxCloseExtensions();
}

void PhysicsMgr::Init()
{
	mPhysX->Init();

	mDispatcher->CreateCpuDispatcher();
	PxSceneDesc sceneDesc(mPhysX->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -0.05f, 0.0f);
	sceneDesc.cpuDispatcher = mDispatcher->GetCpuDispatcher();
	sceneDesc.filterShader = PlayerFilter;
	sceneDesc.simulationEventCallback = mDispatcher->GetSimulationCallback();

	mPhysX->CreatePhysicsScene(sceneDesc);
}

void PhysicsMgr::Update()
{
	GetScene()->simulate(dru::DT);
	GetScene()->fetchResults(true);
}

PxScene* PhysicsMgr::GetScene()
{
	return mPhysX->GetPhysScene()->GetScene();
}
