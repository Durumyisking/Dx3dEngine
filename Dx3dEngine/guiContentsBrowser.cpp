#include "guiContentsBrowser.h"

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

		if (mSceneSaveName.rfind(L".scene") == mSceneSaveName.size() - 6)
			return false;

		activeScene->Save();

		return false;
	}

	bool ContentsBrowser::LoadScene()
	{
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
}