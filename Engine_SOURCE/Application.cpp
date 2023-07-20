#include "Application.h"
#include "Renderer.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "CollisionMgr.h"
#include "PhysXCollisionMgr.h"
#include "FMod.h"
#include "FontWrapper.h"
#include "FileMgr.h"
#include "PhysicsMgr.h"
#include "TimerMgr.h"

namespace dru
{

	Application::Application()
		: mbInitalized(false)
		, mGraphicDevice(nullptr)
		, mHwnd{}
		, mHdc{}
		, mWidth(0)
		, mHeight(0)
		, mHmenu{}
		, mResolution(Vector2::Zero)
	{
	}
	Application::~Application()
	{

	}

	void Application::Initialize()
	{
		GETSINGLE(FileMgr)->TestLoad(L"..//Resources/CityMan.dae");

		GETSINGLE(TimeMgr)->Initialize();
		GETSINGLE(InputMgr)->Initialize();
		GETSINGLE(Fmod)->Initialize();
		// GETSINGLE(CollisionMgr)->Initialize();
		renderer::Initialize();
		GETSINGLE(PhysicsMgr)->Initialize();
		GETSINGLE(FontWrapper)->Initialize();
		GETSINGLE(SceneMgr)->Initialize();

	}
	void Application::Update()
	{
		GETSINGLE(TimeMgr)->Update();

		if (!GETSINGLE(TimeMgr)->IsUpdatePass())
		{
			GETSINGLE(TimerMgr)->Update();
			GETSINGLE(InputMgr)->Update();
			//		GETSINGLE(CollisionMgr)->Update();
			GETSINGLE(SceneMgr)->Update();
			GETSINGLE(PhysXCollisionMgr)->Update();
			GETSINGLE(PhysicsMgr)->Update();
		}
	}
	void Application::FixedUpdate()
	{
		if (!GETSINGLE(TimeMgr)->IsUpdatePass())
		{
			//GETSINGLE(CollisionMgr)->FixedUpdate();
			GETSINGLE(SceneMgr)->FixedUpdate();
		}
	}
	void Application::Render()
	{
		if (!GETSINGLE(TimeMgr)->IsUpdatePass())
		{
			GETSINGLE(TimeMgr)->Render(mHdc);
			GETSINGLE(InputMgr)->Render(mHdc);
			//		CollisionMgr::Render();
			mGraphicDevice->Clear();
			mGraphicDevice->AdjustViewPorts();

			renderer::Render();
			GETSINGLE(SceneMgr)->Render();
			GETSINGLE(SceneMgr)->FontRender();
		}
	}

	void Application::Destroy()
	{
		if (!GETSINGLE(TimeMgr)->IsUpdatePass())
		{
			GETSINGLE(SceneMgr)->Destory();
		}

	}

	void Application::LateEvent()
	{
		if (!GETSINGLE(TimeMgr)->IsUpdatePass())
		{
			GETSINGLE(SceneMgr)->LateEvent();
		}
	}

	void Application::Run()
	{
		Update();
		FixedUpdate();
		Render();

		// 프레임 종료 후 오브젝트 삭제 및 추가
		Destroy();
		LateEvent();
	}

	void Application::Present()
	{
		mGraphicDevice->Present();
	}

	void Application::Release()
	{
		//GETSINGLE(FileMgr)->FileLoad(L"..//Resources/MarioHackStart.SMD");
		GETSINGLE(ResourceMgr)->Release();
		GETSINGLE(Fmod)->Release();
		GETSINGLE(SceneMgr)->Release();
		GETSINGLE(FontWrapper)->Release();
	}

	void Application::DestroySingle()
	{

		GETSINGLE(SceneMgr)->DestroyInstance();
		GETSINGLE(FontWrapper)->DestroyInstance();
//		GETSINGLE(CollisionMgr)->DestroyInstance();
		GETSINGLE(PhysXCollisionMgr)->DestroyInstance();
		GETSINGLE(Fmod)->DestroyInstance();
		GETSINGLE(InputMgr)->DestroyInstance();
		GETSINGLE(TimeMgr)->DestroyInstance();
		GETSINGLE(ResourceMgr)->DestroyInstance();
		GETSINGLE(PhysicsMgr)->DestroyInstance();
		GETSINGLE(TimerMgr)->DestroyInstance();
		GETSINGLE(FileMgr)->DestroyInstance();
	}

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
		if (mGraphicDevice == nullptr)
		{
			mHwnd = hwnd;
			mHdc = GetDC(mHwnd);
			mWidth = width;
			mHeight = height;

			eValidationMode vaildationMode = eValidationMode::Disabled;
			mGraphicDevice = std::make_unique<GraphicDevice>();
			//dru::GetDevice() = mGraphicDevice.get();
		}


		RECT rt = { 0, 0, static_cast<LONG>(width) , static_cast<LONG>(height) };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

	Vector2 Application::GetResolutionRatio()
	{
		RECT	windowRC;

		GetClientRect(mHwnd, &windowRC);

		float width = static_cast<float>(windowRC.right - windowRC.left);
		float height = static_cast<float>(windowRC.bottom - windowRC.top);

		return Vector2(mWidth / width, mHeight / height);

	}


	void Application::DockingMenu()
	{
		SetMenu(mHwnd, mHmenu);
		ChangeWindowSize(true);
	}
	void Application::DivideMenu()
	{
		SetMenu(mHwnd, nullptr);
		ChangeWindowSize(false);
	}
	void Application::ChangeWindowSize(bool bMenu)
	{
		RECT rt = { 0, 0, static_cast<LONG>(mResolution.x), static_cast<LONG>(mResolution.y) };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, bMenu);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	}
}
