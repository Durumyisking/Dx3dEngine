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
#include "guiListWidget.h"

#include "Scene.h"
#include "SceneMgr.h"
#include "PathMgr.h"

extern Application application;
extern gui::Editor editor;

namespace gui
{

	ContentsBrowser::ContentsBrowser()
		: mScene(nullptr)
		, mSceneSaveName()
		, mCurrentSceneName()
		, mSceneSaveType(-1)
	{
		SetName("ContentsBrowser");
		UINT width = 1600;
		UINT height = 900;

		Vector2 size(static_cast<float>(width), static_cast<float>(height));

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		TreeInitialize();
	}

	ContentsBrowser::~ContentsBrowser()
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

	bool ContentsBrowser::SaveScene()
	{
		Scene* activeScene = GETSINGLE(SceneMgr)->GetActiveScene();

		if (activeScene == nullptr)
			return false;

		SceneMgr::eSceneType SceneType = static_cast<SceneMgr::eSceneType>(mSceneSaveType);

		if (SceneType == SceneMgr::eSceneType::End || SceneType == SceneMgr::eSceneType::None)
			return false;

		if (mSceneSaveName.size() < 7)
			return false;

		if (mSceneSaveName.rfind(L".scene") == mSceneSaveName.size() - 6)
		{
			std::wstring fullpath = GETSINGLE(PathMgr)->FindPath(SCENE_SAVE_PATH) + mSceneSaveName;

			if (GETSINGLE(SceneMgr)->SaveSceneFile(SceneType, fullpath))
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
		OpenFile.lpstrFilter = TEXT("모든파일\0*.*");
		OpenFile.lpstrFile = FilePath;	// FilePath 에 풀경로가 들어온다.
		OpenFile.nMaxFile = MAX_PATH;
		OpenFile.lpstrInitialDir = GETSINGLE(PathMgr)->FindPath(SCENE_SAVE_PATH).c_str();

		if (GetOpenFileName(&OpenFile) != 0)
		{
			int	Length = lstrlen(FilePath);

			char	FilePathMultiByte[MAX_PATH] = {};

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

			GETSINGLE(PathMgr)->ResetPath();

			// .scene 파일인지 확인하고 씬 로드
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


		ImGui::Spacing();

		static bool isError = false;

		// Save 버튼
		if (ImGui::Button("Save Scene")) {
			ImGui::OpenPopup("Save Confirmation");
		}

		if (ImGui::BeginPopupModal("Save Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
		{
			ImGui::Text("Save Scene?");

			// Yes 버튼
			if (ImGui::Button("Yes", ImVec2(120, 80)))
			{
				ImGui::CloseCurrentPopup();
				if (!SaveScene())
					isError = true;
				else
					isError = false;
			}
			ImGui::SameLine();

			// No 버튼
			if (ImGui::Button("No", ImVec2(120, 80))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();


		}

		ImGui::SameLine();

		// Load 버튼
		if (ImGui::Button("Load Scene")) {
			ImGui::OpenPopup("Load Confirmation");
		}

		if (ImGui::BeginPopupModal("Load Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
		{
			ImGui::Text("Load Scene?");

			// Yes 버튼
			if (ImGui::Button("Yes", ImVec2(120, 80)))
			{
				ImGui::CloseCurrentPopup();
				if (!LoadScene())
					isError = true;
				else
					isError = false;
			}
			ImGui::SameLine();

			// No 버튼
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

	void SetSceneTypeName(SceneMgr::eSceneType type, std::string& name)
	{
		switch (type)
		{
		case SceneMgr::eSceneType::Title:
			name = "Title";
			break;
		case SceneMgr::eSceneType::Play:
			name = "Play";
			break;
		case SceneMgr::eSceneType::Test:
			name = "Test";
			break;
		case SceneMgr::eSceneType::End:
			name = "End";
			break;
		default:
			name = "Add Name in ContentsBrowser::SetSceneTypeName";
			break;
		}
	}

	void ContentsBrowser::TreeInitialize()
	{
		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("SceneTypeSelect");
		AddWidget(mTreeWidget);

		mTreeWidget->SetSimpleEvent(this
			, std::bind(&ContentsBrowser::SetSceneType, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		mTreeWidget->Clear();

		//Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		std::string sceneName("SceneType : ");
		
		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);
		
		for (UINT i = 0; i < static_cast<UINT>(SceneMgr::eSceneType::End); i++)
		{
			std::string name = {};

			SetSceneTypeName(static_cast<SceneMgr::eSceneType>(i), name);

			SceneMgr::eSceneType sceneType = static_cast<SceneMgr::eSceneType>(i);
			mTreeWidget->AddNode(root, name, 0, false, i);
		}
	}

	void ContentsBrowser::SetSceneType(UINT data)
	{
		mSceneSaveType = data;
	}
}