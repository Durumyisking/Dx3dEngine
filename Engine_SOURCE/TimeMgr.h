#pragma once
#include "Engine.h"


class TimeMgr
{
	SINGLE(TimeMgr)

public:
	__forceinline float DeltaTime() { return mDeltaTime; }
	__forceinline float DeltaTimeConstant() { return mDeltaTimeConstant; }


		 
	void Initialize();
	void Update();
	void Render(HDC hdc);


	bool IsUpdatePass() const { return mbUpdatePass; }

	float GetMaxFrameRate() const { return mMaxFrameRate; }


private:
	void frameRateLock();
	void frameRateLock_Debugging();

private:
	LARGE_INTEGER	mCpuFrequency;
	LARGE_INTEGER	mPrevFrequency;
	LARGE_INTEGER	mCurFrequency;

	float			mDeltaTime;
	float			mDeltaTimeConstant;
	float			mOneSecond;

	float			mbBulletTimeTimer;
	float			mbBulletTimeTimerMax;

	float			mMaxFrameRate;
	float			mFrameRateStack;

	bool			mbUpdatePass;

};
