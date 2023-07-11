#include "PhysicsScene.h"


PhysicsScene::PhysicsScene(PxScene* Scene)
	:mScene(Scene)
{
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::AddActor(PxActor* Actor)
{
	assert(mScene);
	mScene->addActor(*Actor);
}

void PhysicsScene::RemoveActor(PxActor* Actor)
{
	assert(mScene);
	mScene->removeActor(*Actor);
}
