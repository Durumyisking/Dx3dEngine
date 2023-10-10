#include "guiMeshRenderer.h"
#include "MeshRenderer.h"
#include "guiEditor.h"
#include "guiListWidget.h"
#include "ResourceMgr.h"
#include "EngineResource.h"
#include "guiInspector.h"
#include "SpriteRenderer.h"
#include "guiWidgetMgr.h"

#include "Material.h"
#include "Mesh.h"


extern gui::Editor editor;


namespace gui
{
	GUIMeshRenderer::GUIMeshRenderer()
		: GUIComponent(eComponentType::MeshRenderer)
	{
		SetName("MeshRenderer");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	GUIMeshRenderer::~GUIMeshRenderer()
	{

	}

	void GUIMeshRenderer::FixedUpdate()
	{
		GUIComponent::FixedUpdate();

		if (GetTarget())
		{
			MeshRenderer* meshRenderer
				= GetTarget()->GetComponent<MeshRenderer>();

			if (meshRenderer == nullptr)
				return;

			//SpriteRenderer* spriteRenderer
			//	= GetTarget()->GetComponent<SpriteRenderer>();

			//if (spriteRenderer == nullptr)
			//	return;

			if (meshRenderer->GetModel())
				true;
			else
				false;


			mMaterial = meshRenderer->GetMaterial();
			mMesh = meshRenderer->GetMesh();


		}
	}

	void GUIMeshRenderer::Update()
	{
		GUIComponent::Update();

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
			ListWidget* listUI = GETSINGLE(WidgetMgr)->GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			

			//모든 메쉬의 리소스를 가져와야한다.
			std::vector<Mesh*> meshes 
				= GETSINGLE(ResourceMgr)->Finds<Mesh>();

			std::vector<std::wstring> wName;
			for (auto mesh : meshes)
			{
				wName.push_back(mesh->GetName());
			}


			listUI->SetItemList(wName);
			listUI->SetEvent(this, std::bind(&GUIMeshRenderer::SetMesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUI = GETSINGLE(WidgetMgr)->GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			//모든 메쉬의 리소스를 가져와야한다.
			std::vector<Material*> materials
				= GETSINGLE(ResourceMgr)->Finds<Material>();

			std::vector<std::wstring> wName;
			for (auto material : materials)
			{
				wName.push_back(material->GetName());
			}

			listUI->SetItemList(wName);
			listUI->SetEvent(this, std::bind(&GUIMeshRenderer::SetMaterial
				, this, std::placeholders::_1));
		}
	}

	void GUIMeshRenderer::LateUpdate()
	{
		GUIComponent::LateUpdate();
	}

	void GUIMeshRenderer::SetMesh(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(wKey);

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<MeshRenderer>()->SetMesh(mesh);
	}

	void GUIMeshRenderer::SetMaterial(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		Material* material = GETSINGLE(ResourceMgr)->Find<Material>(wKey);

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<MeshRenderer>()->SetMaterial(material);
	}
}
