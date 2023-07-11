#include "PhysicsMgr.h"

#include "PhysX.h"
#include "TimeMgr.h"
#include "PxEventCallback.h"
#include "Engine.h"
#include "PxFilter.h"
#include "PhysicsScene.h"

using namespace dru;

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

void PhysicsMgr::Initialize()
{
	mPhysX->Init();

	mDispatcher->CreateCpuDispatcher();
	PxSceneDesc sceneDesc(mPhysX->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f * 150.f, 0.0f);
	sceneDesc.cpuDispatcher = mDispatcher->GetCpuDispatcher();
	sceneDesc.filterShader = PlayerFilter;
	sceneDesc.simulationEventCallback = mDispatcher->GetSimulationCallback();

	mPhysX->CreatePhysicsScene(sceneDesc);
//	mPhysX->ConnectDebuggerToScene();
}

void PhysicsMgr::Update()
{
	GetScene()->simulate(DT);
	GetScene()->fetchResults(true);
}

PxScene* PhysicsMgr::GetScene()
{
	return mPhysX->GetPhysScene()->GetScene();
}
