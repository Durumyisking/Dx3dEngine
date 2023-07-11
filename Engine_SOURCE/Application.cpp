#include "Application.h"
#include "Renderer.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "CollisionMgr.h"
#include "FMod.h"
#include "FontWrapper.h"
#include "FileMgr.h"
#include "PhysicsMgr.h"


namespace dru
{

	Application::Application()
		: initalized(false)
		, graphicDevice(nullptr)
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
		Fmod::Release();
		SceneMgr::release();
		FontWrapper::Release();
		PhysicsMgr::DestroyInstance();
	}

	void Application::Initialize()
	{
		GETSINGLE(TimeMgr)->Initialize();
		GETSINGLE(InputMgr)->Initialize();
		GETSINGLE(Fmod)->Initialize();
		GETSINGLE(CollisionMgr)->Initialize();
		renderer::Initialize();
		GETSINGLE(PhysicsMgr)->GetInstance()->Init();
		GETSINGLE(FontWrapper)->Initialize();
		GETSINGLE(SceneMgr)->Initialize();
	}
	void Application::update()
	{
		GETSINGLE(TimeMgr)->update();
		GETSINGLE(InputMgr)->update();
		GETSINGLE(PhysicsMgr)->update();
		GETSINGLE(CollisionMgr)->update();
		GETSINGLE(SceneMgr)->update();
	}
	void Application::fixedUpdate()
	{
		GETSINGLE(CollisionMgr)->fixedUpdate();
		GETSINGLE(SceneMgr)->fixedUpdate();
	}
	void Application::render()
	{
		GETSINGLE(TimeMgr)->Render(mHdc);
		GETSINGLE(InputMgr)->Render(mHdc);
		//		CollisionMgr::render();
		graphicDevice->Clear();
		graphicDevice->AdjustViewPorts();

		renderer::Render();
		GETSINGLE(SceneMgr)->render();
		GETSINGLE(SceneMgr)->fontRender();
	}

	void Application::destroy()
	{
		GETSINGLE(SceneMgr)->destory();
	}

	void Application::lateEvent()
	{
		GETSINGLE(SceneMgr)->LateEvent();
	}

	void Application::Run()
	{
		update();
		fixedUpdate();
		render();

		// 프레임 종료 후 오브젝트 삭제 및 추가
		destroy();
		lateEvent();
	}

	void Application::Present()
	{
		graphicDevice->Present();
	}

	void Application::Release()
	{
		GETSINGLE(FileMgr)->FileLoad(L"..//Resources/MarioHackStart.SMD");
		GETSINGLE(Fmod)->Release();
		GETSINGLE(SceneMgr)->release();
		GETSINGLE(FontWrapper)->Release();
		GETSINGLE(ResourceMgr)->Release();
	}

	void Application::DestroySingle()
	{
		GETSINGLE(SceneMgr)->DestroyInstance();
		GETSINGLE(FontWrapper)->DestroyInstance();
		GETSINGLE(CollisionMgr)->DestroyInstance();
		GETSINGLE(Fmod)->DestroyInstance();
		GETSINGLE(InputMgr)->DestroyInstance();
		GETSINGLE(TimeMgr)->DestroyInstance();
		GETSINGLE(ResourceMgr)->DestroyInstance();
	}

	void Application::SetWindow(HWND _hwnd, UINT _width, UINT _height)
	{
		if (graphicDevice == nullptr)
		{
			mHwnd = _hwnd;
			mHdc = GetDC(mHwnd);
			mWidth = _width;
			mHeight = _height;

			eValidationMode vaildationMode = eValidationMode::Disabled;
			graphicDevice = std::make_unique<GraphicDevice>();
			//dru::GetDevice() = graphicDevice.get();
		}

		RECT rt = { 0, 0, (LONG)_width , (LONG)_height };
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
	void Application::ChangeWindowSize(bool _bMenu)
	{
		RECT rt = { 0, 0, static_cast<LONG>(mResolution.x), static_cast<LONG>(mResolution.y) };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	}
}
