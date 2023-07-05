#include "TimeMgr.h"
#include "Application.h"

extern dru::Application application;

namespace dru
{
    LARGE_INTEGER	TimeMgr::mCpuFrequency = {};
    LARGE_INTEGER   TimeMgr::mPrevFrequency = {};
    LARGE_INTEGER	TimeMgr::mCurFrequency = {};
    float			TimeMgr::mDeltaTime = 0.0f;
    float			TimeMgr::mDeltaTimeConstant = 0.0f;
    float			TimeMgr::mOneSecond = 0.0f;
    float			TimeMgr::mAccumulatedTime = 0.0f;
    float			TimeMgr::mbBulletTimeTimer= 0.0f;
    float			TimeMgr::mbBulletTimeTimerMax = 0.0f;
    bool			TimeMgr::mbBulletTime = false;
    bool			TimeMgr::mbPlayerBulletTime = false;
    UINT			TimeMgr::mFramePass = 0;
    UINT			TimeMgr::mFramePassCount = 0;
    bool            TimeMgr::mbFramePassCheck = true;

    void TimeMgr::Initialize()
    {
        //CPU 의 초당 반복되는 주파수를 얻어온다.
        QueryPerformanceFrequency(&mCpuFrequency);

        //프로그램을 시작했을때의 CPU 클럭 수
        QueryPerformanceCounter(&mPrevFrequency);
    }

    void TimeMgr::update()
    {
        QueryPerformanceCounter(&mCurFrequency);

        float differenceInFrequancy
            = static_cast<float>((mCurFrequency.QuadPart - mPrevFrequency.QuadPart));

        mDeltaTime = differenceInFrequancy / static_cast<float>(mCpuFrequency.QuadPart);
        mDeltaTimeConstant = mDeltaTime;

        if (mbBulletTime)
        {
            if (mbFramePassCheck)
            {
                mFramePassCount = 0;
                mbFramePassCheck = false;
            }

            mbBulletTimeTimer += mDeltaTime;
            mDeltaTime /= 3.f;


            if (0 != mbBulletTimeTimerMax)
            {
                if (mbBulletTimeTimer > mbBulletTimeTimerMax)
                {
                    mbPlayerBulletTime = false;
                    mbBulletTime = false;
                    mbBulletTimeTimerMax = 0.f;
                    mbBulletTimeTimer = 0.f;
                }
            }
        }
        else
        {
            if (0 != mFramePassCount && mbFramePassCheck)
            {
                mFramePassCount = 0;
                mbFramePassCheck = false;
            }
        }

        mPrevFrequency.QuadPart = mCurFrequency.QuadPart;

        mAccumulatedTime += mDeltaTime;

        if (mFramePass == mFramePassCount)
        {
            mbFramePassCheck = true;
        }
        if (mFramePass > 0)
        {
            ++mFramePassCount;
        }


#ifdef _DEBUG
        if (mDeltaTime > (1.f / 60.f))
            mDeltaTime = (1.f / 60.f);
        if (mDeltaTimeConstant > (1.f / 60.f))
            mDeltaTimeConstant = (1.f / 60.f);
#endif
    }

    void TimeMgr::Render(HDC hdc)
    {
        static int iCount = 0;
        ++iCount;

        // 1 초에 한번
        mOneSecond += mDeltaTime;
        if (1.0f < mOneSecond)
        {
            HWND hWnd = application.GetHwnd();

            wchar_t szFloat[50] = {};
            float FPS = 1.f / mDeltaTime;
            swprintf_s(szFloat, 50, L"Fps : %d", iCount);
            int iLen = static_cast<int>(wcsnlen_s(szFloat, 50));
            //TextOut(_dc, 10, 10, szFloat, iLen);

            SetWindowText(hWnd, szFloat);

            // 누적시간, 카운트 초기화
            mOneSecond = 0.f;
            iCount = 0;
        }


    }

    void TimeMgr::BulletTime(float _Time)
    {
        mbBulletTime = true;
        mbBulletTimeTimerMax = _Time;
    }

}