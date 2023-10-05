#include "guiContentsBrowser.h"

#include "Application.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"

#include "guiInspector.h"
#include "guiResource.h"
#include "guiEditor.h"
#include "guiWidgetMgr.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "PathMgr.h"

extern Application application;
extern gui::Editor editor;

namespace gui
{

	ContentsBrowser::ContentsBrowser()
		: mScene(nullptr)
		, mSceneSaveName()
		, mCurrentSceneName()
	{
		SetName("ContentsBrowser");
		UINT width = 1600;
		UINT height = 900;

		Vector2 size(static_cast<float>(width), static_cast<float>(height));

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));
	}

	ContentsBrowser::~ContentsBrowser()
	{
	}

	bool ContentsBrowser::SaveScene()
	{
		Scene* activeScene = GETSINGLE(SceneMgr)->GetActiveScene();

		if (activeScene == nullptr)
			return false;

		if (mSceneSaveName.size() < 7)
			return false;

		if (mSceneSaveName.rfind(L".scene") == mSceneSaveName.size() - 6)
		{
			std::wstring fullpath = GETSINGLE(PathMgr)->FindPath(SCENE_SAVE_PATH) + mSceneSaveName;

			if (GETSINGLE(SceneMgr)->SaveSceneFile(fullpath))
				return true;
		}

		return false;
	}

	bool ContentsBrowser::LoadScene()
	{
		TCHAR	FilePath[MAX_PATH] = {};

		OPENFILENAME	OpenFile = {};

		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.hwndOwner = application.GetHwnd();
		OpenFile.lpstrFilter = TEXT("�������\0*.*");
		OpenFile.lpstrFile = FilePath;	// FilePath �� Ǯ��ΰ� ���´�.
		OpenFile.nMaxFile = MAX_PATH;
		OpenFile.lpstrInitialDir = GETSINGLE(PathMgr)->FindPath(SCENE_SAVE_PATH).c_str();

		if (GetOpenFileName(&OpenFile) != 0)
		{
			int	Length = lstrlen(FilePath);

			char	FilePathMultiByte[MAX_PATH] = {};

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

			GETSINGLE(PathMgr)->ResetPath();

			// .scene �������� Ȯ���ϰ� �� �ε�
			if (lstrcmpi(FilePath + Length - 6, TEXT(".scene")) == 0) 
			{
				if (GETSINGLE(SceneMgr)->LoadSceneFile(ConvertToW_String(FilePathMultiByte)))
					return true;
			}
		}

		GETSINGLE(PathMgr)->ResetPath();

		return false;
	}

	void ContentsBrowser::Initialize()
	{
	}

	void ContentsBrowser::FixedUpdate()
	{
		Scene* activeScene = GETSINGLE(SceneMgr)->GetActiveScene();

		mCurrentSceneName = activeScene->GetName();
	}

	void ContentsBrowser::Update()
	{
		static char buffer[256] = "";

		if (ImGui::InputText("SceneSaveName", buffer, IM_ARRAYSIZE(buffer)))
		{
			if (buffer[0] != NULL)
				mSceneSaveName = StringToWStringReturn(buffer);
		}

		static bool isError = false;

		// Save ��ư
		if (ImGui::Button("Save Scene")) {
			ImGui::OpenPopup("Save Confirmation");
		}

		if (ImGui::BeginPopupModal("Save Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
		{
			ImGui::Text("Save Scene?");

			// Yes ��ư
			if (ImGui::Button("Yes", ImVec2(120, 80)))
			{
				ImGui::CloseCurrentPopup();
				if (!SaveScene())
					isError = true;
				else
					isError = false;
			}
			ImGui::SameLine();

			// No ��ư
			if (ImGui::Button("No", ImVec2(120, 80))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();


		}

		// Load ��ư
		if (ImGui::Button("Load Scene")) {
			ImGui::OpenPopup("Load Confirmation");
		}

		if (ImGui::BeginPopupModal("Load Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
		{
			ImGui::Text("Load Scene?");

			// Yes ��ư
			if (ImGui::Button("Yes", ImVec2(120, 80)))
			{
				ImGui::CloseCurrentPopup();
				if (!LoadScene())
					isError = true;
				else
					isError = false;
			}
			ImGui::SameLine();

			// No ��ư
			if (ImGui::Button("No", ImVec2(120, 80))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if(isError)
			ImGui::OpenPopup("Failed");

		// if Failed;
		if (ImGui::BeginPopupModal("Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Work Failed.");

			if (ImGui::Button("OK.", ImVec2(80, 40)))
			{
				ImGui::CloseCurrentPopup();
				isError = false;
			}
			ImGui::EndPopup();
		}
	}

	void ContentsBrowser::LateUpdate()
	{

	}
}