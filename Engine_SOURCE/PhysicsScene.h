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

private:
	// ���� �ùķ��̼� �����̳�
	// �浹�� ���⼭ �����ϰ� ó������
	PxScene* mScene;
};
