#include "PhysicsScene.h"
#include "PhysicsMgr.h"

PhysicsScene::PhysicsScene(PxScene* scene)
	:mScene(scene)
{
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::AddActor(PxActor* actor)
{
	assert(mScene);
	mScene->addActor(*actor);
}

void PhysicsScene::RemoveActor(PxActor* actor)
{
	assert(mScene);
	mScene->removeActor(*actor);
}

void PhysicsScene::CreateDynamic(const PxTransform& transform, const PxGeometry& geometry, PxMaterial& material, const PxVec3& velocity)
{
	PxPhysics* physics =  GETSINGLE(PhysicsMgr)->GetEnvironment()->GetPhysics();
	PxRigidDynamic* dynamic = PxCreateDynamic(*physics, transform, geometry, material, 10.f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
}
