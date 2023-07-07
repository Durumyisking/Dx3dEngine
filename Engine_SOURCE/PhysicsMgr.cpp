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
PxSimulationFilterShader ��ӹ޴� Ŭ���� �����
������� PxQueryFilterCallback ��ӹ޾Ҵµ�;;

*/


void PhysicsMgr::Init()
{
	mPhysX->Init();

	mDispatcher->CreateCpuDispatcher();
	PxSceneDesc sceneDesc(mPhysX->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -0.05f, 0.0f);
	sceneDesc.cpuDispatcher = mDispatcher->GetCpuDispatcher();
//	sceneDesc.filterShader = PlayerFilterShader;

	// ���Ŀ� �ڽ� Ŭ������ �޾ƾ���
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
