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
		GroupWidget* group1 = new GroupWidget("ConsoleGroup1");
		AddWidget(group1);

		ImageWidget* image1 = new ImageWidget("Image1");
		image1->SetSize(100.f, 100.f);
		group1->AddWidget(image1);

		ImageWidget* image2 = new ImageWidget("Image2");
		image2->SetTexture(L"default");
		image2->SetSize(100.f, 100.f);
		group1->AddWidget(image2);

		ImageWidget* image3 = new ImageWidget("Image3");
		image3->SetTexture(L"default");
		image3->SetSize(100.f, 100.f);
		group1->AddWidget(image3);

		ImageWidget* image4 = new ImageWidget("Image4");
		image4->SetTexture(L"default");
		image4->SetSize(100.f, 100.f);
		group1->AddWidget(image4);


		//GUITexture* image2 = new gui::GUITexture();
		//image2->SetName("Image2");
		//AddWidget(image2);

		//image2->SetTarget(GETSINGLE(ResourceMgr)->Find<Texture>(L"default"));
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

	void Console::Render()
	{
		Widget::Render();
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
