#include "guiMeshRenderer.h"
#include "MeshRenderer.h"
#include "guiEditor.h"
#include "guiListWidget.h"
#include "Resources.h"
#include "Resource.h"
#include "guiInspector.h"
#include "SpriteRenderer.h"

extern gui::Editor editor;

namespace gui
{
	MeshRenderer::MeshRenderer()
		: Component(eComponentType::MeshRenderer)
	{
		SetName("MeshRenderer");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::FixedUpdate()
	{
		Component::FixedUpdate();

		if (GetTarget())
		{
			dru::MeshRenderer* meshRenderer
				= GetTarget()->GetComponent<dru::MeshRenderer>();

			if (meshRenderer == nullptr)
				return;

			//dru::SpriteRenderer* spriteRenderer
			//	= GetTarget()->GetComponent<dru::SpriteRenderer>();

			//if (spriteRenderer == nullptr)
			//	return;


			mMaterial = meshRenderer->GetMaterial();
			mMesh = meshRenderer->GetMesh();
		}
	}

	void MeshRenderer::Update()
	{
		Component::Update();

		if (mMesh == nullptr
			|| mMaterial == nullptr)
			return;

		std::string meshName
			= std::string(mMesh->GetName().begin(), mMesh->GetName().end());
		std::string materialName
			= std::string(mMaterial->GetName().begin(), mMaterial->GetName().end());

		ImGui::Text("Mesh"); 
		ImGui::InputText("##MeshName", (char*)meshName.data()
			, meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("##MeshBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUI = editor.GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			

			//모든 메쉬의 리소스를 가져와야한다.
			std::vector<std::shared_ptr<dru::Mesh>> meshes 
				= dru::Resources::Finds<dru::Mesh>();

			std::vector<std::wstring> wName;
			for (auto mesh : meshes)
			{
				wName.push_back(mesh->GetName());
			}

			listUI->SetItemList(wName);
			listUI->SetEvent(this, std::bind(&MeshRenderer::SetMesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUI = editor.GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			//모든 메쉬의 리소스를 가져와야한다.
			std::vector<std::shared_ptr<dru::Material>> materials
				= dru::Resources::Finds<dru::Material>();

			std::vector<std::wstring> wName;
			for (auto material : materials)
			{
				wName.push_back(material->GetName());
			}

			listUI->SetItemList(wName);
			listUI->SetEvent(this, std::bind(&MeshRenderer::SetMaterial
				, this, std::placeholders::_1));
		}
	}

	void MeshRenderer::LateUpdate()
	{
		Component::LateUpdate();
	}

	void MeshRenderer::SetMesh(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		std::shared_ptr<dru::Mesh> mesh = dru::Resources::Find<dru::Mesh>(wKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<dru::MeshRenderer>()->SetMesh(mesh);
	}

	void MeshRenderer::SetMaterial(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		std::shared_ptr<dru::Material> material = dru::Resources::Find<dru::Material>(wKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<dru::MeshRenderer>()->SetMaterial(material);
	}
}
