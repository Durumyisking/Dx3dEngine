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
		: mFolders(nullptr)
		, mTargetPath()
		, mTargetFolders()
	{
		SetName("Project");
		UINT width = 1600;
		UINT height = 900;

		Vector2 size(static_cast<float>(width), static_cast<float>(height));

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		mFolders = new GroupWidget();
		mFolders->SetName("ProjectFolderGroup");
		AddWidget(mFolders);

		////GroupWidget Template
		//mFolders->SetCollpase(true);
		mFolders->SetSpacing();
		mFolders->SetNextLine(5);

		//folder button
		ButtonWidget* folderButton = new ButtonWidget("OpenFolder");
		folderButton->SetSize(100.f, 50.f);

		folderButton->SetClickCallback(this, &Project::OpenFolderCallback);

		AddWidget(folderButton);

		ResetContent();
	}

	Project::~Project()
	{
		mFolders->Clear();

		delete mFolders;
		mFolders = nullptr;
	}

	void Project::FixedUpdate()
	{
		Widget::FixedUpdate();

		//리소스가 바뀐다면 리소스목록 초기화
	}

	void Project::Update()
	{
		Widget::Update();

		ImGui::Text(mTargetPath.c_str());
	}

	void Project::LateUpdate()
	{

	}

	void Project::ResetContent()
	{
		//mTreeWidget->Close();
		mFolders->Clear();

		//SetDefaultPath;
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path().parent_path();
		mTargetPath = parentPath.string() + "\\Resources";

	}

	void Project::FolderClickCallback(UINT index, std::string path)
	{
		ResetContent();

		//std::string path = mTargetFolders[index];

		SelectFolder(path);
	}

	void Project::OpenFolderCallback()
	{
		ResetContent();

		SelectFolder(mTargetPath);
	}
	
	void Project::SelectFolder(std::string& path)
	{
		TCHAR	FilePath[MAX_PATH] = {};

		OPENFILENAME	OpenFile = {};

		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.hwndOwner = application.GetHwnd();
		OpenFile.lpstrFilter = TEXT("모든파일\0*.*");
		OpenFile.lpstrFile = FilePath;	// FilePath 에 풀경로가 들어온다.
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

			// #ifdef : 뒤에 있는 내용이 #define으로 정의되어 있는지를
			// 판단하는 if문이다. 
			// 컴파일 단계에서 뒤에 있는 내용이 #define으로 정의되어 있는지 판단.
#ifdef UNICODE

	// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를
	// 얻어온다.
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

			// TCHAR 문자열을 std::wstring으로 변환
			std::wstring wstr(FilePath);
			// std::wstring을 std::string으로 변환 (UTF-8 인코딩으로)
			std::string utf8String(wstr.begin(), wstr.end());

			mTargetPath = utf8String;

			mTargetFolders.clear();

			int maxDepth = std::count(mTargetPath.begin(), mTargetPath.end(), '\\') + 1;

			for (const auto& entry : std::filesystem::recursive_directory_iterator(FilePathMultiByte))
			{
				if (entry.is_directory()) // Check if it's a directory
				{
					std::string folderPath = entry.path().generic_string();
					size_t startPos = folderPath.find(Directory);

					if (startPos != std::string::npos)
					{
						std::string folderName = folderPath.substr(startPos + DirectoryLength);
						// 폴더의 깊이를 추적하여 하위 폴더를 제외합니다.
						int folderDepth = std::count(folderName.begin(), folderName.end(), '/');
						if (folderDepth == 0) // 상위 폴더만 저장
						{
							mTargetFolders.push_back(folderName);
						}
					}
				}
			}

			Texture* folderImage = GETSINGLE(ResourceMgr)->Find<Texture>(L"FolderImage");
			UINT index = 0;
			// Now, folderNames vector contains the names of folders in the selected directory
			for (const std::string& folderName : mTargetFolders)
			{
				ButtonWidget* button = mFolders->CreateWidget<ButtonWidget>();

				button->SetText(folderName);
				button->SetTexture(folderImage);

				button->SetClickCallback([this, index, folderName]() {
					FolderClickCallback(index, folderName);
					});

				index++;
			}
		}
	}

	//void Project::OpenFolderCallback()
	//{
	//	CoInitialize(NULL);

	//	BROWSEINFO bi = { 0 };
	//	bi.lpszTitle = TEXT("Select a folder");
	//	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	//	if (pidl != nullptr) {
	//		TCHAR path[MAX_PATH];
	//		if (SHGetPathFromIDList(pidl, path)) {
	//			// 선택한 폴더 경로가 path에 저장됩니다.
	//			// 이제 선택한 폴더를 사용할 수 있습니다.
	//		}
	//		CoTaskMemFree(pidl); // 메모리 해제
	//	}

	//	CoUninitialize();
	//}

	void Project::toConsole()
	{
		//Resource* resource = static_cast<Resource*>(data);

		Console* console = GETSINGLE(WidgetMgr)->GetWidget<Console>("Console");
		console->SetTargetProjectPath(mTargetPath);
		console->InitializeTargetProjectPath();
	}

}