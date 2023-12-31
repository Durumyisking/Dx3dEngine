#pragma once
#include "Engine.h"


class PxEventCallback;

class PxDispatcher
{
public:
	PxDispatcher();
	~PxDispatcher();

public:
	PxDefaultCpuDispatcher* GetCpuDispatcher() { return mCpuDispatcher; }
	PxEventCallback* GetSimulationCallback() { return mCallback; }
	void	CreateCpuDispatcher(UINT numThreads);

private:
	PxDefaultCpuDispatcher* mCpuDispatcher;
	PxEventCallback* mCallback;
};

