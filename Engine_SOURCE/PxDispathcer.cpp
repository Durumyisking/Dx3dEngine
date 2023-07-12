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

void PxDispatcher::CreateCpuDispatcher(UINT numThreads)
{
	mCpuDispatcher = PxDefaultCpuDispatcherCreate(numThreads); // 인자로 쓰레드 개수가 들어간다.
}

