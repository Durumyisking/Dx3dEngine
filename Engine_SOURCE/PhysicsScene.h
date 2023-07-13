#pragma once
#include "Engine.h"


class PhysicsScene
{
public:
	PhysicsScene(PxScene* scene);
	~PhysicsScene();

public:
	PxScene* GetScene() { return mScene; }

public:
	void AddActor(PxActor* actor);
	void RemoveActor(PxActor* actor);
	
	void CreateDynamic(const PxTransform& transform, const PxGeometry& geometry, PxMaterial& material, const PxVec3& velocity);

private:
	// 물리 시뮬레이션 컨테이너
	// 충돌도 여기서 감지하고 처리해줌
	PxScene* mScene;
};
