#pragma once
#include "Engine.h"


namespace dru
{
	class TimeMgr
	{
		SINGLE(TimeMgr)

	public:
		__forceinline float DeltaTime() { return mDeltaTime; }
		__forceinline float DeltaTimeConstant() { return mDeltaTimeConstant; }
		__forceinline float AccumulatedTime() { return mAccumulatedTime; }
		__forceinline void BulletTimeOn() 
		{
			mFramePass = 3;
			mFramePassCount = 0;
			mbBulletTime = true;
			mbFramePassCheck = false;
		}
		__forceinline void PlayerBulletTimeOn()
		{
			mbPlayerBulletTime = true;
		}
		__forceinline void BulletTimeOff()
		{
			mFramePass = 0;
			mFramePassCount = 0;
			mbBulletTime = false;
			mbFramePassCheck = true;
		}
		__forceinline void PlayerBulletTimeOff()
		{
			mbPlayerBulletTime = false;
		}
		__forceinline bool IsBulletTimeOn() { return mbBulletTime; }
		__forceinline bool IsPlayerBulletTimeOn() { return mbPlayerBulletTime; }

		__forceinline void SetFramePass(UINT value) { mFramePass = value; }
		__forceinline bool IsFramePass() 
		{
			return mbFramePassCheck; 
		}

		 
		void Initialize();
		void Update();
		void Render(HDC hdc);
		void BulletTime(float time);



	private:
		LARGE_INTEGER	mCpuFrequency;
		LARGE_INTEGER	mPrevFrequency;
		LARGE_INTEGER	mCurFrequency;

		float			mDeltaTime;
		float			mDeltaTimeConstant;
		float			mOneSecond;
		float			mAccumulatedTime;

		float			mbBulletTimeTimer;
		float			mbBulletTimeTimerMax;
		bool				mbBulletTime;// 설정한 FramePass가 지나면 true가 되고 다음 프레임에 false가 됩니다
		bool				mbPlayerBulletTime; // shift를 눌러서 킨 bullettime일때만 true가 됩니다.

		UINT				mFramePass; // 설정한 FramePass만큼의 Frame이 지났는지 확인합니다
		UINT				mFramePassCount;
		bool				mbFramePassCheck; // 설정한 FramePass가 지나면 true가 되고 다음 프레임에 false가 됩니다.

	};
}