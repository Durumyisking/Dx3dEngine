#include "guiMeshRenderer.h"
#include "MeshRenderer.h"
#include "guiEditor.h"
#include "guiListWidget.h"
#include "ResourceMgr.h"
#include "Resource.h"
#include "guiInspector.h"
#include "SpriteRenderer.h"

#include "Material.h"
#include "Mesh.h"

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
			std::vector<dru::Mesh*> meshes 
				= GETSINGLE(dru::ResourceMgr)->Finds<dru::Mesh>();

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
			std::vector<dru::Material*> materials
				= GETSINGLE(dru::ResourceMgr)->Finds<dru::Material>();

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
		dru::Mesh* mesh = GETSINGLE(dru::ResourceMgr)->Find<dru::Mesh>(wKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<dru::MeshRenderer>()->SetMesh(mesh);
	}

	void MeshRenderer::SetMaterial(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		dru::Material* material = GETSINGLE(dru::ResourceMgr)->Find<dru::Material>(wKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<dru::MeshRenderer>()->SetMaterial(material);
	}
}
