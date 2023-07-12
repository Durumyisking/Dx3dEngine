#include "PhysicsScene.h"


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
