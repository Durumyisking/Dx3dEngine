#pragma once
#include "Engine.h"


class PhysicsScene
{
public:
	PhysicsScene(PxScene* Scene);
	~PhysicsScene();

public:
	PxScene* GetScene() { return mScene; }

public:
	void AddActor(PxActor* Actor);
	void RemoveActor(PxActor* Actor);

private:
	// ���� �ùķ��̼� �����̳�
	// �浹�� ���⼭ �����ϰ� ó������
	PxScene* mScene;
};
