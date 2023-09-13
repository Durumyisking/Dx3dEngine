#include "guiProject.h"

#include "Application.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "ResourceMgr.h"

#include "guiInspector.h"
#include "guiResource.h"
#include "guiEditor.h"
#include "guiWidgetMgr.h"
#include "guiConsole.h"
#include "guiImageWidget.h"
#include "guiButtonWidget.h"
#include <ShlObj.h>

extern gui::Editor editor;
extern Application application;

namespace gui
{

	Project::Project()
		: mGroup(nullptr)
		, mbTargetChanged(false)
		, mTargetPath()
		, mTargetFolders()
	{
		SetName("Project");
		UINT width = 1600;
		UINT height = 900;

		Vector2 size(static_cast<float>(width), static_cast<float>(height));

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		//SetDefaultPath;
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path().parent_path();
		mTargetPath = parentPath.string() + "\\Resources";

		//folder button
		ButtonWidget* folderButton = new ButtonWidget("OpenFolder");
		folderButton->SetSize(80.f, 20.f); 

		folderButton->SetClickCallback(&Project::OpenFolderCallback, this, mTargetPath);

		AddWidget(folderButton);


		////GroupWidget Template
		mGroup = new GroupWidget();
		mGroup->SetName("ProjectFolderGroup");
		AddWidget(mGroup);

		//mFolders->SetCollpase(true);
		mGroup->SetSpacing();
		mGroup->SetNextLine(5);

	}

	Project::~Project()
	{
		mGroup->Clear();

		delete mGroup;
		mGroup = nullptr;
	}

	void Project::FixedUpdate()
	{
		Widget::FixedUpdate();

		//���ҽ��� �ٲ�ٸ� ���ҽ���� �ʱ�ȭ
	}

	void Project::Update()
	{
		Widget::Update();

		ImGui::Text(mTargetPath.c_str());

		ImGui::SameLine();
	}

	void Project::LateUpdate()
	{
		if (mGroup)
		{
			if (mbTargetChanged)
			{
				mGroup->Clear();

				Texture* folderImage = GETSINGLE(ResourceMgr)->Find<Texture>(L"FolderImage");

				for (std::string folderName : mTargetFolders)
				{
					ButtonWidget* button = mGroup->CreateWidget<ButtonWidget>(50.f, 50.f);

					button->SetText(folderName);
					button->SetTexture(folderImage);

					button->SetClickCallback(&Project::FolderClickCallback, this, folderName);
				}

				toConsole();

				mbTargetChanged = false;
			}
		}
	}

	void Project::FolderClickCallback(std::string path)
	{
		const std::string& fullpath = mTargetPath + path;

		SelectFolder(fullpath);

		mbTargetChanged = true;
	}

	void Project::OpenFolderCallback(const std::string& path)
	{
		SelectFolder(path);

		mbTargetChanged = true;
	}
	
	void Project::SelectFolder(const std::string& path)
	{
		TCHAR	FilePath[MAX_PATH] = {};

		OPENFILENAME	OpenFile = {};

		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.hwndOwner = application.GetHwnd();
		OpenFile.lpstrFilter = TEXT("�������\0*.*");
		OpenFile.lpstrFile = FilePath;	// FilePath �� Ǯ��ΰ� ���´�.
		OpenFile.nMaxFile = MAX_PATH;
		std::wstring wstr(path.begin(), path.end());
		OpenFile.lpstrInitialDir = wstr.c_str();

		if (GetOpenFileName(&OpenFile) != 0)
		{
			int	Length = lstrlen(FilePath);

			for (int i = Length - 1; i >= 0; --i)
			{
				if (FilePath[i] == '/' || FilePath[i] == '\\')
				{

					memset(&FilePath[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
					break;
				}
			}

			char	FilePathMultiByte[MAX_PATH] = {};

	// #ifdef �ڿ� �ִ� ������ #define���� ���ǵǾ� �ִ����� �Ǵ��ϴ� if��
	// ������ �ܰ迡�� �ڿ� �ִ� ������ #define���� ���ǵǾ� �ִ��� �Ǵ�
#ifdef UNICODE
	// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ����
	// ���´�.
			int	PathLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
				0, 0, 0, 0);

			WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
				FilePathMultiByte, PathLength, 0, 0);

#else

			strcpy_s(FilePathMultiByte, FilePath);

#endif // UNICODE

			char	FolderName[13] = {};

			char	Directory[128] = {};

			int	FilePathLength = (int)strlen(FilePathMultiByte);

			if (FilePathLength >= 12)
			{
				for (int i = FilePathLength - 13; i >= 0; --i)
				{
					memcpy(FolderName, &FilePathMultiByte[i], 12);

					//Resources���� ��ġ�� �޾ƿ������� ���������� ������Ʈ ������ ������ �Ǿ� �ִµ� ����ɼ��� ������ Resources���� �ȿ� ������ �ϳ� �� ����°� ���ƺ���
					if (strcmp(FolderName, "\\Dx3dEngine\\") == 0)
					{
						strcpy_s(Directory, &FilePathMultiByte[i + 12]);
						break;
					}
				}
			}

			int DirectoryLength = (int)strlen(Directory);

			for (int i = DirectoryLength; i >= 0; --i)
			{
				if (Directory[i] == '\\')
					Directory[i] = '/';
			}

			// TCHAR ���ڿ��� std::wstring���� ��ȯ
			std::wstring wstr(FilePath);
			// std::wstring�� std::string���� ��ȯ (UTF-8 ���ڵ�����)
			std::string utf8String(wstr.begin(), wstr.end());

			mTargetPath = utf8String;

			mTargetFolders.clear();
			mTargetFiles.clear();

			int maxDepth = std::count(mTargetPath.begin(), mTargetPath.end(), '\\') - 1;

			for (const auto& entry : std::filesystem::recursive_directory_iterator(FilePathMultiByte))
			{
				if (entry.is_directory()) // Check if it's a directory
				{
					std::string folderPath = entry.path().generic_string();
					size_t startPos = folderPath.find(Directory);

					if (startPos != std::string::npos)
					{
						std::string folderName = folderPath.substr(startPos + DirectoryLength);
						// ������ ���̸� �����Ͽ� ���� ������ ����
						int folderDepth = std::count(folderName.begin(), folderName.end(), '/');
						if (folderDepth == 0) // ���� ������ ����
						{
							mTargetFolders.push_back(folderName);
						}
					}
				}
				else
				{
					std::string filePath = entry.path().generic_string();
					size_t startPos = filePath.find(Directory);

					if (startPos != std::string::npos)
					{
						std::string fileName = filePath.substr(startPos + DirectoryLength);
						// ������ ���̸� �����Ͽ� ���� ������ ����
						int folderDepth = std::count(fileName.begin(), fileName.end(), '/');
						if (folderDepth == 0) // ���� ������ ����
						{
							mTargetFiles.push_back(fileName);
						}
					}
				}
			}
		}
	}

	void Project::toConsole()
	{
		//Resource* resource = static_cast<Resource*>(data);

		Console* console = GETSINGLE(WidgetMgr)->GetWidget<Console>("Console");
		console->SetTargetProjectPath(mTargetPath);
		console->SetTargetFiles(mTargetFiles);
		console->InitializeTargetProject();
	}

}