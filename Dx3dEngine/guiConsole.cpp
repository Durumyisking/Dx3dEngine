#include "guiConsole.h"
#include "guiGroupWidget.h"
#include "guiImageWidget.h"
#include "guiTexture.h"
#include "ResourceMgr.h"

namespace gui
{
	Console::Console()
	{
		SetName("Console");
	}

	Console::~Console()
	{
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (mChilds[i])
			{
				delete mChilds[i];
				mChilds[i] = nullptr;
			}
		}

		mChilds.clear();
	}

	void Console::Initialize()
	{
		ImageWidget* image1 = new ImageWidget("Image1");
		AddWidget(image1);
		image1->SetTexture(L"default");

		GUITexture* image2 = new gui::GUITexture();
		image2->SetName("Image2");
		AddWidget(image2);

		image2->SetTarget(GETSINGLE(ResourceMgr)->Find<Texture>(L"default"));
	}

	void Console::FixedUpdate()
	{
		Widget::FixedUpdate();
	}

	void Console::Update()
	{
		Widget::Update();
	}

	void Console::LateUpdate()
	{
		Widget::LateUpdate();
	}

	void Console::SetProject(std::wstring path)
	{
	}

	void Console::SetProject(std::string path)
	{
	}

	void Console::Clear()
	{
	}

}
