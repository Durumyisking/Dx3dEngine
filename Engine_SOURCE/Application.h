#pragma once
#include "Engine.h"
#include "GraphicDevice.h"


namespace dru
{
	class Application
	{
	public:
		Application();
		~Application();

		virtual void Initialize();
		// 캐릭터 / 로직 업데이트
		virtual void update();
		// gpu 업데이트
		virtual void fixedUpdate();
		virtual void render();
		virtual void destroy();
		virtual void lateEvent();
		// engine loop
		void Run();
		void Present();

		void Release();
		void DestroySingle();// 프로그램 종료 시점에 호출하여 싱글톤 인스턴스 해제


		void SetWindow(HWND hwnd, UINT width, UINT height);

		void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
		HWND GetHwnd() const { return mHwnd; }

		UINT GetWidth() const { return mWidth; }
		UINT GetHeight() const { return mHeight; }
		math::Vector2 GetResolutionRatio();

		void DockingMenu();
		void DivideMenu();
		void ChangeWindowSize(bool bMenu);

		void SetResolution(math::Vector2 resolution) { mResolution = resolution; };
		math::Vector2 WinResolution() const { return mResolution; }


	private:
		bool mbInitalized = false;
		std::unique_ptr<dru::GraphicDevice> mGraphicDevice;

		HWND mHwnd;
		HDC	 mHdc;
		UINT mWidth;
		UINT mHeight;

		// 메뉴
		HMENU	mHmenu; // Tool Scene에서만 사용하고싶은 menu
		math::Vector2 mResolution;
	};
}

