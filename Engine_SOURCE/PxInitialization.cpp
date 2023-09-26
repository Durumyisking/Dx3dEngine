#include "PxInitialization.h"


PxInitialization::PxInitialization()
	: mAllocCallback{}
	, mErrorCallback{}
	, mFoundation{}
	, mPhysics{}
	, mPvd{}
	, mTransport{}
	, mCooking{}
{
}

PxInitialization::~PxInitialization()
{
	if (mPhysics)
		mPhysics->release();

	if (mPvd)
		mPvd->release();

	if (mTransport)
		mTransport->release();

	if (mCooking)
		mCooking->release();

	if (mFoundation)
		mFoundation->release();
}

void PxInitialization::CreateCooking()
{
	physx::PxTolerancesScale scale = physx::PxTolerancesScale();

	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(scale));
	if (!mCooking)
		assert("PxCreateCooking failed!");
}

void PxInitialization::CreateFoundation()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocCallback, mErrorCallback);
	assert(mFoundation);
}

void PxInitialization::CreatePhysics()
{
	bool recordMemoryAllocations = true;

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), recordMemoryAllocations, mPvd);

//#ifdef _DEBUG
//	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), recordMemoryAllocations, mPvd);
//
//#endif
//	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale());

	assert(mPhysics);
}

void PxInitialization::CreateVisualDebugger()
{
	mPvd = PxCreatePvd(*mFoundation);
	assert(mPvd);
}

void PxInitialization::ConntectVisualDebugger()
{
	mTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	assert(mTransport);

	if (mPvd)
	{
		mPvd->connect(*mTransport, PxPvdInstrumentationFlag::eALL);
	}
}


