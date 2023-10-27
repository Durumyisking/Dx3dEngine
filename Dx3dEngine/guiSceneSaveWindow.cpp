#include "guiSceneSaveWindow.h"

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

#include "guiTreeWidget.h"
#include "guiGroupWidget.h"


extern Application application;
extern gui::Editor editor;

namespace gui
{

	SceneSaveWindow::SceneSaveWindow()
		: mScene(nullptr)
		, mSceneSaveName()
		, mCurrentSceneName()
		, mSceneSaveType(-1)
		, mLayerSaveType(-1)
		, mSceneTree(nullptr)
		, mLayerTree(nullptr)
		, mGroupWidget(nullptr)
	{
		SetName("SceneSaveWindow");
		UINT width = 1600;
		UINT height = 900;

		Vector2 size(static_cast<float>(width), static_cast<float>(height));

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));
	}

	SceneSaveWindow::~SceneSaveWindow()
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

	bool SceneSaveWindow::SaveScene()
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

	bool SceneSaveWindow::LoadScene()
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

	bool SceneSaveWindow::SaveLayerObjects()
	{
		Scene* activeScene = GETSINGLE(SceneMgr)->GetActiveScene();

		if (activeScene == nullptr)
			return false;

		if (mLayerSaveType == -1)
			return false;

		eLayerType layerType = static_cast<eLayerType>(mLayerSaveType);

		if (layerType == eLayerType::End)
			return false;
		
		if (mSceneSaveName.size() < 7)
			return false;

		if (mSceneSaveName.rfind(L".layer") == mSceneSaveName.size() - 6)
		{
			std::wstring fullpath = GETSINGLE(PathMgr)->FindPath(LAYER_SAVE_PATH) + mSceneSaveName;

			if (GETSINGLE(SceneMgr)->SaveLayerObjects(layerType, fullpath))
				return true;
		}

		return false;
	}

	bool SceneSaveWindow::LoadLayerObjects()
	{
		TCHAR	FilePath[MAX_PATH] = {};

		OPENFILENAME	OpenFile = {};

		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.hwndOwner = application.GetHwnd();
		OpenFile.lpstrFilter = TEXT("모든파일\0*.*");
		OpenFile.lpstrFile = FilePath;	// FilePath 에 풀경로가 들어온다.
		OpenFile.nMaxFile = MAX_PATH;
		OpenFile.lpstrInitialDir = GETSINGLE(PathMgr)->FindPath(LAYER_SAVE_PATH).c_str();

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
			if (lstrcmpi(FilePath + Length - 6, TEXT(".layer")) == 0)
			{
				if (mLayerSaveType == -1)
					return false;

				eLayerType layerType = static_cast<eLayerType>(mLayerSaveType);

				if (layerType == eLayerType::End)
					return false;

				if (GETSINGLE(SceneMgr)->LoadLayerObjects(layerType, ConvertToW_String(FilePathMultiByte)))
				{
					GETSINGLE(WidgetMgr)->ForceReset();
					return true;
				}
			}
		}

		GETSINGLE(PathMgr)->ResetPath();

		return false;
	}

	void SetSceneTypeName(SceneMgr::eSceneType type, std::string& name)
	{
		switch (type)
		{
		case SceneMgr::eSceneType::Loading:
			name = "Loading";
			break;
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
			name = "Add Name in SceneSaveWindow::SetSceneTypeName";
			break;
		}
	}

	void SceneSaveWindow::Initialize()
	{
		//mTreeWidget = new TreeWidget();
		//mTreeWidget->SetName("SceneTypeSelect");
		//AddWidget(mTreeWidget);

		//mTreeWidget->SetSimpleEvent(this
		//	, std::bind(&SceneSaveWindow::SetSceneType, this, std::placeholders::_1));

		//mTreeWidget->SetDummyRoot(true);

		//mTreeWidget->Clear();

		////Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		//std::string sceneName("SceneType : ");

		//TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);

		//for (UINT i = 0; i < static_cast<UINT>(SceneMgr::eSceneType::End); i++)
		//{
		//	std::string name = {};

		//	SetSceneTypeName(static_cast<SceneMgr::eSceneType>(i), name);

		//	SceneMgr::eSceneType sceneType = static_cast<SceneMgr::eSceneType>(i);
		//	mTreeWidget->AddNode(root, name, 0, false, i);
		//}

		mGroupWidget = new GroupWidget();
		mGroupWidget->SetName("SaveWindowListGroup");
		mGroupWidget->SetSpacing(5.f, 5.f);
		mGroupWidget->SetNextLine(2);
		AddWidget(mGroupWidget);

		{
			mSceneTree = mGroupWidget->CreateWidget<TreeWidget>(130.f, 140.f);
			mSceneTree->SetName("SceneTypeSelect");

			mSceneTree->SetSimpleEvent(this
				, std::bind(&SceneSaveWindow::SetSceneType, this, std::placeholders::_1));

			mSceneTree->SetDummyRoot(true);

			mSceneTree->Clear();

			std::string sceneName("SceneType : ");

			TreeWidget::Node* root = mSceneTree->AddNode(nullptr, sceneName, 0, true);

			for (UINT i = 0; i < static_cast<UINT>(SceneMgr::eSceneType::End); i++)
			{
				std::string name = {};

				SetSceneTypeName(static_cast<SceneMgr::eSceneType>(i), name);

				SceneMgr::eSceneType sceneType = static_cast<SceneMgr::eSceneType>(i);
				mSceneTree->AddNode(root, name, 0, false, i);
			}
		}

		{
			mLayerTree = mGroupWidget->CreateWidget<TreeWidget>(130.f, 140.f);
			mLayerTree->SetName("LayerTypeSelect");

			mLayerTree->SetSimpleEvent(this
				, std::bind(&SceneSaveWindow::SetLayerType, this, std::placeholders::_1));

			mLayerTree->SetDummyRoot(true);

			mLayerTree->Clear();

			std::string layerName("LayerType : ");

			TreeWidget::Node* root = mLayerTree->AddNode(nullptr, layerName, 0, true);

			for (int i = 0; i < static_cast<int>(enums::eLayerType::End); i++)
			{
				std::string layerTypeName = enums::charLayerType[i];

				mLayerTree->AddNode(root, layerTypeName, 0, false, i);
			}
		}
	}

	void SceneSaveWindow::FixedUpdate()
	{
		Scene* activeScene = GETSINGLE(SceneMgr)->GetActiveScene();

		mCurrentSceneName = activeScene->GetName();
	}

	void SceneSaveWindow::Update()
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

		ImGui::SameLine();

		// Layer Save 버튼
		if (ImGui::Button("Save Layer")) {
			ImGui::OpenPopup("Save Layer Confirmation");
		}

		if (ImGui::BeginPopupModal("Save Layer Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Save Layer?");

			// Yes 버튼
			if (ImGui::Button("Yes", ImVec2(120, 80)))
			{
				ImGui::CloseCurrentPopup();
				if (!SaveLayerObjects())
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

		// Layer Load 버튼
		if (ImGui::Button("Load Layer")) {
			ImGui::OpenPopup("Load Layer Confirmation");
		}

		if (ImGui::BeginPopupModal("Load Layer Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Load Layer?");

			// Yes 버튼
			if (ImGui::Button("Yes", ImVec2(120, 80)))
			{
				ImGui::CloseCurrentPopup();
				if (!LoadLayerObjects())
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

	void SceneSaveWindow::LateUpdate()
	{

	}

	void SceneSaveWindow::SetSceneType(UINT data)
	{
		mSceneSaveType = data;
	}

	void SceneSaveWindow::SetLayerType(UINT data)
	{
		mLayerSaveType = data;
	}
}