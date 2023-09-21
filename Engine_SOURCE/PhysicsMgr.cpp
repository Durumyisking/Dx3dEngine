#include "PhysicsMgr.h"

#include "PhysX.h"
#include "TimeMgr.h"
#include "PxEventCallback.h"
#include "Engine.h"
#include "PxFilter.h"
#include "PhysicsScene.h"
#include "TimeMgr.h"


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

	mDispatcher->CreateCpuDispatcher(1);

	PxSceneDesc sceneDesc(mPhysX->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.simulationEventCallback = mDispatcher->GetSimulationCallback();
	sceneDesc.filterShader = PlayerFilter;
	//sceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKILL;
	sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
	//sceneDesc.flags = PxSceneFlag::eADAPTIVE_FORCE;
	sceneDesc.cpuDispatcher = mDispatcher->GetCpuDispatcher();	
	
	mPhysX->CreatePhysicsScene(sceneDesc);
	mPhysX->ConnectDebuggerToScene();

	assert(mPhysX->GetPhysicsScene());
}

void PhysicsMgr::Update()
{
	GetScene()->simulate(GETSINGLE(TimeMgr)->GetMaxFrameRate());
	GetScene()->fetchResults(true);
}

PxScene* PhysicsMgr::GetScene()
{
	return mPhysX->GetPhysicsScene()->GetScene();
}
