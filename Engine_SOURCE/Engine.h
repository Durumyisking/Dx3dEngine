#pragma once

#include <winsock2.h>
#include "def.h"
#include "CommonInclude.h"
#include "druEnums.h"
#include "StringFunctions.h"
#include "Macro.h"

//#include <PxPhysicsAPI.h>
#pragma warning( push )
#pragma warning( disable : 4705 )
#pragma warning( disable : 4706 )
#pragma warning( disable : 4707 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 33010 )
//
//#define PX_PHYSX_STATIC_LIB
//#include "../External/PhysX/Include/PxConfig.h"
#include "../External/PhysX/Include/PxPhysicsAPI.h"
#include "druMath.h"
#pragma warning( pop ) 


#ifdef _DEBUG


#pragma comment(lib, "../External/PhysX/lib/debug/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCooking_64.lib")

//Recast && Detour
#pragma comment(lib, "../External/Recast/lib/debug/Recast-d.lib")
#pragma comment(lib, "../External/Detour/lib/debug/Detour-d.lib")
#pragma comment(lib, "../External/Detour/lib/debug/DetourCrowd-d.lib")
#pragma comment(lib, "../External/Detour/lib/debug/DetourTileCache-d.lib")
#pragma comment(lib, "../External/Detour/lib/debug/DebugUtils-d.lib")

#else


#pragma comment(lib, "../External/PhysX/lib/release/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCooking_64.lib")

//Recast && Detour
#pragma comment(lib, "../External/Recast/lib/release/Recast.lib")
#pragma comment(lib, "../External/Detour/lib/release/Detour.lib")
#pragma comment(lib, "../External/Detour/lib/release/DetourCrowd.lib")
#pragma comment(lib, "../External/Detour/lib/release/DetourTileCache.lib")
#pragma comment(lib, "../External/Detour/lib/release/DebugUtils.lib")
#endif

using namespace physx;
using namespace enums;


// ImGui_internal.h
// ImGui.h 보다 먼저 선언 되어야 하므로 미리 선언 해둠
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif




struct Timer
{
public:
	Timer(float endTime)
		:mElapsedTime(0.f)
		, mEndTime(endTime)
		, mbRunning(false)
		, mbFinished(true)
		, mbDestroy(true)
		, function (nullptr)
	{}

	~Timer()
	{
		
	}

	void Reset()
	{
		mElapsedTime = 0.f;
		mbRunning = false;
		mbFinished = false;
	}

	void Restart()
	{
		mElapsedTime = 0.f;
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

				if (function) // Todo : 각 오브젝트들의 Update부분에서 실행해야함
				{
					function();
				}
			}
		}
	}

	void SetEndTime(float endTime)
	{
		mEndTime = endTime;
	}

	bool IsRunning() const 
	{
		return mbRunning;
	}

	bool IsFinished() const
	{
		return mbFinished;
	}
	void SetFinish(bool flag)
	{
		mbFinished = flag;
	}

	bool IsDestroy() const
	{
		return mbDestroy;
	}

	void SetDestroy(bool flag)
	{
		mbDestroy = flag;
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

	void SetFunction(void(*func)())
	{
		function = func;
	}
	std::function<void()>& Event() { return function; }

private:
	float mElapsedTime;
	float mEndTime;
	bool  mbRunning;
	bool  mbFinished;
	bool  mbDestroy;

	std::function<void()> function;

	
};


