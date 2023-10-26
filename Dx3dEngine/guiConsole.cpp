#include "guiConsole.h"
#include "ResourceMgr.h"
#include "Texture.h"

#include "guiTexture.h"
#include "guiImageWidget.h"
#include "guiButtonWidget.h"

namespace gui
{
	Console::Console()
		: mResources{}
		, mGroup(nullptr)
		, mTargetProjectPath()
	{
		SetName("Console");

		////GroupWidget Template
		mGroup = new GroupWidget();
		mGroup->SetName("ConsoleFileGroup");
		AddWidget(mGroup);

		//mFolders->SetCollpase(true);
		mGroup->SetSpacing();
		mGroup->SetNextLine(6);
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

	}

	void Console::FixedUpdate()
	{
	}

	void Console::Update()
	{
	}

	void Console::LateUpdate()
	{
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

	void Console::InitializeTargetProject()
	{
		mGroup->Clear();

		const std::string TextureExtArray[6] = { ".png" , ".PNG" , ".jpg" , ".jpeg" , ".JPG" , ".JPEG" };

		for (std::string fileName : mTargetFiles)
		{
			std::string fileExtension;
			eFileType type = eFileType::None;

			for (size_t i = 0; i < fileName.length(); i++)
			{
				if (fileName[i] == '.')
				{
					fileExtension = fileName.substr(i);
				}
			}

			ButtonWidget* button = mGroup->CreateWidget<ButtonWidget>(110.f, 110.f);

			button->SetText(fileName);
			button->SetButtonText(fileExtension);


			for (int i = 0; i < 6; ++i)
			{
				if (fileExtension == TextureExtArray[i])
				{
					std::string str = mTargetProjectPath + fileName;
					std::wstring wstr(str.begin(), str.end());
					std::wstring wFileName(fileName.begin(), fileName.end());
					Texture* tex = GETSINGLE(ResourceMgr)->LoadFullpath<Texture>(wFileName, wstr);
					button->SetTexture(tex);

					type = eFileType::Texture;
				}
			}

			button->SetClickCallback(&Console::FileClickCallback, this, fileName, type);
		}
	}

	void Console::FileClickCallback(const std::string& name, eFileType type)
	{

	}

}
