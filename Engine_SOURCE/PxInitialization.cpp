#include "PxInitialization.h"


PxInitialization::PxInitialization()
	: mAllocCallback{}
	, mErrorCallback{}
	, mFoundation{}
	, mPhysics{}
{
}

PxInitialization::~PxInitialization()
{
	if (mPhysics)
		mPhysics->release();

	if (mFoundation)
		mFoundation->release();
}

void PxInitialization::CreateFoundation()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocCallback, mErrorCallback);
	assert(mFoundation);
}

void PxInitialization::CreatePhysics()
{
	//bool recordMemoryAllocations = true;

	//mPvd = PxCreatePvd(*gFoundation);
	//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	//mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale());
	assert(mPhysics);
}


