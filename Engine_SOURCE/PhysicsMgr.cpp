#include "PhysicsMgr.h"

#include "PhysX.h"
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
/*
Todo
PxSimulationFilterShader 상속받는 클래스 만들기
현모씬느 PxQueryFilterCallback 상속받았는데;;

*/


void PhysicsMgr::Init()
{
	mPhysX->Init();

	mDispatcher->CreateCpuDispatcher();
	PxSceneDesc sceneDesc(mPhysX->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -0.05f, 0.0f);
	sceneDesc.cpuDispatcher = mDispatcher->GetCpuDispatcher();
//	sceneDesc.filterShader = PlayerFilterShader;

	// 추후에 자식 클래스로 받아야함
	//sceneDesc.simulationEventCallback = mDispatcher->GetSimulationCallback();

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
