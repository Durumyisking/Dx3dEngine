#include "TimeMgr.h"
#include "Application.h"
#include "FontWrapper.h"

extern Application application;



TimeMgr::TimeMgr()
    : mCpuFrequency{}
    , mPrevFrequency{}
    , mCurFrequency{}
    , mDeltaTime(0.0f)
    , mDeltaTimeConstant(0.0f)
    , mOneSecond(0.0f)
    , mbBulletTimeTimer(0.0f)
    , mbBulletTimeTimerMax(0.0f)
    , mMaxFrameRate(1.f / 60.f)
    , mFrameRateStack(0.f)
    , mbUpdatePass(false)

{
}

TimeMgr::~TimeMgr()
{

}

void TimeMgr::Initialize()
{
    //CPU �� �ʴ� �ݺ��Ǵ� ���ļ��� ���´�.
    QueryPerformanceFrequency(&mCpuFrequency);

    //���α׷��� ������������ CPU Ŭ�� ��
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

#ifdef _DEBUG
    frameRateLock();
    frameRateLock_Debugging();
#endif
}

void TimeMgr::Render(HDC hdc)
{
    static wchar_t szFloat[50] = {}; 
    static int iCount = 0;
    ++iCount;

    // 1 �ʿ� �ѹ�
    mOneSecond += mDeltaTimeConstant;
    if (1.0f < mOneSecond)
    {
        HWND hWnd = application.GetHwnd();

        //wchar_t szFloat[50] = {};
        float FPS = 1.f / mDeltaTimeConstant;
        swprintf_s(szFloat, 50, L"Fps : %d", iCount);
        int iLen = static_cast<int>(wcsnlen_s(szFloat, 50));
        //TextOut(_dc, 10, 10, szFloat, iLen);
        SetWindowText(hWnd, szFloat);

        // �����ð�, ī��Ʈ �ʱ�ȭ
        mOneSecond = 0.f;
        iCount = 0;
    }

    GETSINGLE(FontWrapper)->DrawFont(szFloat, 50.f, 800.f, 50.f, FONT_RGBA(255, 255, 255, 255));

}

void TimeMgr::frameRateLock()
{
    // ������ ����
    // �������� �ʹ� ���Ƽ� DT�� max DT���� ������
    if (mMaxFrameRate > mDeltaTime)
    {
        // FrameRateStack�� ����
        // DT = 1.f / max �ع����� cpu �̳� ���� ���µ� max������ó�� �ع����°�
        if (mMaxFrameRate > mFrameRateStack)
        {
            mFrameRateStack += mDeltaTime;
            mbUpdatePass = true;
        }
        else
        {
            // stack�� max�������� DT�� ���ų� ������ max������ó�� ���
            mDeltaTime = mMaxFrameRate;
            mDeltaTimeConstant = mMaxFrameRate;

            mbUpdatePass = false;
            mFrameRateStack = 0.f;
        }
    }
    else
    {
        // max%���� �������� �ȳ����� �׳� ��Ҵ�� ����
        mbUpdatePass = false;
        mFrameRateStack = 0.f;
    }
}

void TimeMgr::frameRateLock_Debugging()
{
        if (mDeltaTime > (1.f / 60.f))
            mDeltaTime = (1.f / 60.f);
}

