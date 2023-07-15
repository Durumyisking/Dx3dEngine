#include "TimeMgr.h"
#include "Application.h"

extern dru::Application application;

namespace dru
{
    TimeMgr::TimeMgr()
        : mCpuFrequency{}
        , mPrevFrequency{}
        , mCurFrequency{}
        , mDeltaTime(0.0f)
        , mDeltaTimeConstant(0.0f)
        , mOneSecond(0.0f)
        , mbBulletTimeTimer(0.0f)
        , mbBulletTimeTimerMax(0.0f)
        , mMaxFrameRate(1.f / 144.f)
        , mFrameRateStack(0.f)
        , mbUpdatePass(false)

    {
    }

    TimeMgr::~TimeMgr()
    {

    }

    void TimeMgr::Initialize()
    {
        //CPU 의 초당 반복되는 주파수를 얻어온다.
        QueryPerformanceFrequency(&mCpuFrequency);

        //프로그램을 시작했을때의 CPU 클럭 수
        QueryPerformanceCounter(&mPrevFrequency);
    }

    void TimeMgr::Update()
    {
        QueryPerformanceCounter(&mCurFrequency);

        float differenceInFrequancy
            = static_cast<float>((mCurFrequency.QuadPart - mPrevFrequency.QuadPart));

        mDeltaTime = differenceInFrequancy / static_cast<float>(mCpuFrequency.QuadPart);
        mDeltaTimeConstant = mDeltaTime;

        
        mPrevFrequency.QuadPart = mCurFrequency.QuadPart;

        frameRateLock();
        frameRateLock_Debugging();
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

    void TimeMgr::frameRateLock()
    {
        // 프레임 고정
        if (mMaxFrameRate > mDeltaTime)
        {
            if (mMaxFrameRate > mFrameRateStack)
            {
                mFrameRateStack += mDeltaTime;
                mbUpdatePass = true;
            }
            else
            {
                mDeltaTime = mMaxFrameRate;

                mbUpdatePass = false;
                mFrameRateStack = 0.f;
            }
        }
        else
        {
            mbUpdatePass = false;
            mFrameRateStack = 0.f;
        }
    }

    void TimeMgr::frameRateLock_Debugging()
    {

//#ifdef _DEBUG
//        if (mDeltaTime > (1.f / 60.f))
//            mDeltaTime = (1.f / 60.f);
//        if (mDeltaTimeConstant > (1.f / 60.f))
//            mDeltaTimeConstant = (1.f / 60.f);
//#endif
    }

}