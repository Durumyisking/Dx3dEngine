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
		bool				mbBulletTime;// ������ FramePass�� ������ true�� �ǰ� ���� �����ӿ� false�� �˴ϴ�
		bool				mbPlayerBulletTime; // shift�� ������ Ų bullettime�϶��� true�� �˴ϴ�.

		UINT				mFramePass; // ������ FramePass��ŭ�� Frame�� �������� Ȯ���մϴ�
		UINT				mFramePassCount;
		bool				mbFramePassCheck; // ������ FramePass�� ������ true�� �ǰ� ���� �����ӿ� false�� �˴ϴ�.

	};
}