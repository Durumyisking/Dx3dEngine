#pragma once

#include "Engine.h"
#include "PhysX.h"
#include "PxDispathcer.h"


class PhysicsMgr
{
	SINGLE(PhysicsMgr)

public:
	void Init();
	void Update();

	std::shared_ptr<PhysX>			GetEnvironment() { return mPhysX; }
	std::shared_ptr<PxDispatcher>   GetDispatcher() { return mDispatcher; }

private:
	PxScene* GetScene();

private:
	std::shared_ptr<PhysX>			mPhysX;
	std::shared_ptr<PxDispatcher>   mDispatcher;

};

