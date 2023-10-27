#include "PathMgr.h"

PathMgr::PathMgr()
{

}

PathMgr::~PathMgr()
{
	mPathMap.clear();
}

bool PathMgr::Initialize()
{
    TCHAR Root[MAX_PATH] = {};

    GetModuleFileName(NULL, Root, MAX_PATH);

    TCHAR FolderName[MAX_PATH] = {};  // 충분한 크기로 변경

    int FilePathLength = static_cast<int>(lstrlen(Root));

    TCHAR target[] = TEXT("\\Dx3dEngine\\");
    int targetLength = lstrlen(target);

    if (FilePathLength >= targetLength)
    {
        for (int i = FilePathLength - targetLength; i >= 0; --i)
        {
            _tcsncpy_s(FolderName, &Root[i], targetLength);

            if (_tcscmp(FolderName, target) == 0)
            {
                Root[i + targetLength] = 0;  // 문자열 종료
                break;
            }
        }
    }

#ifdef UNICODE
    std::wstring rootPath(Root);
#else
    // 멀티 바이트 문자열을 와이드 문자열로 변환
    int length = MultiByteToWideChar(CP_ACP, 0, Root, -1, NULL, 0);
    std::wstring rootPath(length, 0);
    MultiByteToWideChar(CP_ACP, 0, Root, -1, &rootPath[0], length);
#endif

    mPathMap.insert(std::make_pair(ROOT_PATH, rootPath));

    AddPath(RESOURCE_PATH, L"Resources\\");
    AddPath(SAVE_PATH, L"Resources\\SaveFiles\\");
    AddPath(SCENE_SAVE_PATH, L"Scene\\", SAVE_PATH);
    AddPath(MESH_SAVE_PATH, L"Mesh\\", SAVE_PATH);
    AddPath(OBJ_SAVE_PATH, L"Obj\\", SAVE_PATH);
    AddPath(LAYER_SAVE_PATH, L"Layer\\", SAVE_PATH);
    //AddPath(SOUND_PATH, L"Sound\\");
    //AddPath(FONT_PATH, L"Font\\");

    return true;
}

bool PathMgr::AddPath(const std::string& Name, const std::wstring& Path, const std::string& BasePathName)
{
	if (FindPath(Name) == L"NoPath")
		return false;

	std::wstring basePath = FindPath(BasePathName);

	std::wstring newPath = basePath + Path;

	mPathMap.insert(std::make_pair(Name, newPath));

	return true;
}

const std::wstring& PathMgr::FindPath(const std::string& Name)
{
	auto	iter = mPathMap.find(Name);

	if (iter == mPathMap.end())
		return L"NoPath";

	return iter->second;
}

void PathMgr::ResetPath()
{
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::filesystem::path exePath(buffer);

    // 실행 파일의 디렉토리를 현재 작업 디렉토리로 설정
    std::filesystem::current_path(exePath.parent_path());
}
