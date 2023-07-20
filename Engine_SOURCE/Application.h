#pragma once
#include "Engine.h"
#include "GraphicDevice.h"


class Application
{
public:
	Application();
	~Application();

	virtual void Initialize();
	// ĳ���� / ���� ������Ʈ
	virtual void Update();
	// gpu ������Ʈ
	virtual void FixedUpdate();
	virtual void Render();
	virtual void Destroy();
	virtual void LateEvent();
	// engine loop
	void Run();
	void Present();

	void Release();
	void DestroySingle();// ���α׷� ���� ������ ȣ���Ͽ� �̱��� �ν��Ͻ� ����


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
	std::unique_ptr<GraphicDevice> mGraphicDevice;

	HWND mHwnd;
	HDC	 mHdc;
	UINT mWidth;
	UINT mHeight;

	// �޴�
	HMENU	mHmenu; // Tool Scene������ ����ϰ���� menu
	math::Vector2 mResolution;
};


