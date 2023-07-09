#pragma once
#include "Engine.h"
#include "PxInitialization.h"
#include "PhysicsScene.h"
//#include "PxSceneQuery.h"


class PhysX
{
public:
	PhysX();
	~PhysX();

public:
	void Init();

public:
	PxFoundation* GetFoundation() const { return mInitialization->GetFoundation(); }
	PxPhysics* GetPhysics() const { return mInitialization->GetPhysics(); }
	PxControllerManager* GetControllerManager() const { return mControllerMgr; }

	std::shared_ptr<PhysicsScene>	GetPhysScene() const { return mPhysicsScene; }
	//shared_ptr<PxSceneQuery>		 GetSceneQuery() { return m_pSceneQuery; }

	void CreatePhysicsScene(const PxSceneDesc& sceneDesc);

private:
	void CreateScene(const PxSceneDesc& sceneDesc);
	void CreateControllerManager();
	//	void CreateSceneQuery();

private:
	std::shared_ptr<PhysicsScene>		 mPhysicsScene;
	std::shared_ptr<PxInitialization> mInitialization;
	//shared_ptr<PxSceneQuery>		 m_pSceneQuery;

	PxScene* mScene;
	PxControllerManager* mControllerMgr;
};

