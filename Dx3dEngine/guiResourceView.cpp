//#include "guiResourceView.h"
//
//namespace gui
//{
//	ResourceView::ResourceView()
//		: Widget()
//		, mTextureList(nullptr)
//		, mFileList(nullptr)
//		, mTextures{}
//		, mFolderInput{}
//		, mSelectFolderName{}
//	{
//		SetName("ResourceView");
//		SetSize(ImVec2(1600 / 4, 900 / 4));
//
//		mTextureList = new ListWidget();
//		mTextureList->SetName("TextureList");
//		AddWidget(mTextureList);
//
//		mTextureList->SetEvent(this
//			, std::bind(&ResourceView::LoadTexture, this, std::placeholders::_1));
//
//		mTextureList->SetDummyRoot(true);
//
//		InitializeScene();
//	}
//	ResourceView::~ResourceView()
//	{
//	}
//	void ResourceView::Initialize()
//	{
//	}
//	void ResourceView::FixedUpdate()
//	{
//	}
//	void ResourceView::Update()
//	{
//	}
//	void ResourceView::LateUpdate()
//	{
//	}
//	void ResourceView::LoadTextureList()
//	{
//	}
//	void ResourceView::TextureClickCallback(int Index, const std::string& Item)
//	{
//	}
//	void ResourceView::OpenTextureFileCallback()
//	{
//	}
//	void ResourceView::OpenTextureFolderCallback()
//	{
//	}
//}
//
//
//void CResourceWindow::OpenTextureFolderCallback()
//{
//	TCHAR	FilePath[MAX_PATH] = {};
//
//	OPENFILENAME	OpenFile = {};
//
//	OpenFile.lStructSize = sizeof(OPENFILENAME);
//	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
//	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0JPGFile\0*.jpg\0PNGFile\0*.png\0BmpFile\0*.bmp");
//	OpenFile.lpstrFile = FilePath;	// FilePath 에 풀경로가 들어온다.
//	OpenFile.nMaxFile = MAX_PATH;
//	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ROOT_PATH)->Path;
//
//	if (GetOpenFileName(&OpenFile) != 0)
//	{
//		int	Length = lstrlen(FilePath);
//
//		for (int i = Length - 1; i >= 0; --i)
//		{
//			if (FilePath[i] == '/' || FilePath[i] == '\\')
//			{
//
//				memset(&FilePath[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
//				break;
//			}
//		}
//
//		char	FilePathMultiByte[MAX_PATH] = {};
//
//		// #ifdef : 뒤에 있는 내용이 #define으로 정의되어 있는지를
//		// 판단하는 if문이다. 
//		// 컴파일 단계에서 뒤에 있는 내용이 #define으로 정의되어 있는지 판단.
//#ifdef UNICODE
//
//	// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를
//	// 얻어온다.
//		int	PathLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
//			0, 0, 0, 0);
//
//		WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
//			FilePathMultiByte, PathLength, 0, 0);
//
//#else
//
//		strcpy_s(FilePathMultiByte, FilePath);
//
//#endif // UNICODE
//
//		char	FolderName[6] = {};
//
//		char	Directory[128] = {};
//
//		int	FilePathLength = (int)strlen(FilePathMultiByte);
//
//		if (FilePathLength >= 5)
//		{
//			for (int i = FilePathLength - 6; i >= 0; --i)
//			{
//				memcpy(FolderName, &FilePathMultiByte[i], 5);
//
//				if (strcmp(FolderName, "\\Bin\\") == 0)
//				{
//					strcpy_s(Directory, &FilePathMultiByte[i + 5]);
//					break;
//				}
//			}
//		}
//
//		int DirectoryLength = (int)strlen(Directory);
//
//		for (int i = DirectoryLength; i >= 0; --i)
//		{
//			if (Directory[i] == '\\')
//				Directory[i] = '/';
//		}
//
//		//TextureList 설정
//		mTextureList->Clear();
//
//		m_FolderInput->SetText(Directory);
//
//		const char* TextureExtArray[5] = { ".BMP", ".JPG", ".PNG", ".TGA", ".DDS" };
//
//		for (const auto& file : std::filesystem::recursive_directory_iterator(FilePathMultiByte))
//		{
//			char	Name[256] = {};
//			char	FullPath[MAX_PATH] = {};
//			char	Ext[_MAX_EXT] = {};
//			char	FolderName[128] = {};
//
//			strcpy_s(FullPath, file.path().generic_string().c_str());
//
//			int	Length = (int)strlen(FullPath);
//
//			if (Length >= DirectoryLength)
//			{
//				for (int i = Length - DirectoryLength - 1; i >= 0; --i)
//				{
//					memcpy(FolderName, &FullPath[i], DirectoryLength);
//
//					if (strcmp(FolderName, Directory) == 0)
//					{
//						strcpy_s(Name, &FullPath[i + DirectoryLength]);
//						break;
//					}
//				}
//			}
//
//			_splitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);
//
//			_strupr_s(Ext);
//
//			bool	FileExt = false;
//
//			for (int i = 0; i < 5; ++i)
//			{
//				if (strcmp(Ext, TextureExtArray[i]) == 0)
//				{
//					FileExt = true;
//					break;
//				}
//			}
//
//			if (!FileExt)
//				continue;
//
//			mTextureList->AddItem(Name);
//
//			char	FilePath[MAX_PATH] = {};
//
//			strcat_s(FilePath, FilePathMultiByte);
//
//			strcat_s(FilePath, Name);
//
//			TCHAR	FinalPath[MAX_PATH] = {};
//
//			Length = (int)MultiByteToWideChar(CP_ACP, 0, FilePath, -1, 0, 0);
//			MultiByteToWideChar(CP_ACP, 0, FilePath, -1, FinalPath, Length);
//
//			CResourceManager::GetInst()->LoadTextureFullPath(Name, FinalPath);
//		}
//	}
//}
