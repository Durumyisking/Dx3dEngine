#include "PxDispathcer.h"
#include "PxEventCallback.h"


PxDispatcher::PxDispatcher()
	: mCpuDispatcher(nullptr)
	, mCallback(nullptr)
{
	mCallback = new PxEventCallback;
}

PxDispatcher::~PxDispatcher()
{
	if (mCpuDispatcher)
		mCpuDispatcher->release();

	if (mCallback)
	{
		delete mCallback;
		mCallback = nullptr;
	}
}

void PxDispatcher::CreateCpuDispatcher()
{
	mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
}

