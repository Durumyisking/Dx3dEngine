#pragma once

#include "def.h"
#include "CommonInclude.h"
#include "druEnums.h"
#include "StringFunctions.h"
#include "Macro.h"
#include "druMath.h"

//#include <PxPhysicsAPI.h>
#include "../External/PhysX/Include/PxPhysicsAPI.h"


#ifdef _DEBUG


#pragma comment(lib, "../External/PhysX/lib/debug/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCharacterKinematic_static_64.lib")
#else


#pragma comment(lib, "../External/PhysX/lib/release/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCharacterKinematic_static_64.lib")
#endif

using namespace physx;


// ImGui_internal.h
// ImGui.h 보다 먼저 선언 되어야 하므로 미리 선언 해둠
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

using namespace dru::enums;


struct Timer
{
public:
	Timer(float endTime)
		:mElapsedTime(0.f)
		, mEndTime(endTime)
		, mbRunning(false)
		, mbFinished(true)
	{}

	void Reset()
	{
		mElapsedTime = 0.f;
		mbRunning = false;
		mbFinished = false;
	}

	void Start()
	{
		mElapsedTime = 0.f;
		mbRunning = true;
		mbFinished = false;
	}

	void Stop()
	{
		mElapsedTime = 0.f;
		mbRunning = false;
		mbFinished = false;
	}

	void Update(float deltaTime)
	{
		if (mbFinished)
			return;

		if (mbRunning)
		{
			mElapsedTime += deltaTime;
			if (mElapsedTime > mEndTime)
			{
				mElapsedTime = mEndTime;
				mbFinished = true;
			}
		}
	}

	void SetEndTime(float endTime)
	{
		mEndTime = endTime;
	}

	bool IsRunning()
	{
		return mbRunning;
	}

	bool IsFinished()
	{
		return mbFinished;
	}

	float GetProgress() const
	{
		if (mEndTime == 0.f)
			return 1.f;

		return std::clamp(mElapsedTime / mEndTime, 0.f, 1.f);
	}

	void SetProgress(float progress)
	{
		mElapsedTime = mEndTime * progress;
	}

private:
	float mElapsedTime;
	float mEndTime;
	bool  mbRunning;
	bool  mbFinished;

};

