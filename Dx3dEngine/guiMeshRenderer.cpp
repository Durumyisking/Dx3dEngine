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
		SetSize(ImVec2(250.0f, 400.0f));
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
			mModel = meshRenderer->GetModel();
		}
	}

	void GUIMeshRenderer::Update()
	{
		GUIComponent::Update();

		std::string meshName = "NoMeshes";
		std::string materialName = "NoMaterial";
		std::string modelName = "NoModel";

		if (mMesh)
			meshName = std::string(mMesh->GetName().begin(), mMesh->GetName().end());
		if (mMaterial)
			materialName = std::string(mMaterial->GetName().begin(), mMaterial->GetName().end());
		if (mModel)
			modelName = std::string(mModel->GetName().begin(), mModel->GetName().end());

		ImGui::Text("Mesh"); 
		ImGui::InputText("##MeshName", (char*)meshName.data()
			, meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("##MeshBtn", ImVec2(45.0f, 30.0f)))
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
		if (ImGui::Button("##MaterialBtn", ImVec2(45.0f, 30.0f)))
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

		ImGui::Text("Model"); //ImGui::SameLine();
		ImGui::InputText("##Moel", (char*)modelName.data()
			, modelName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##ModelBtn", ImVec2(45.0f, 30.0f)))
		{
			ListWidget* listUI = GETSINGLE(WidgetMgr)->GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			//모든 메쉬의 리소스를 가져와야한다.
			std::vector<Model*> models
				= GETSINGLE(ResourceMgr)->Finds<Model>();

			std::vector<std::wstring> wName;
			for (auto model : models)
			{
				wName.push_back(model->GetName());
			}

			listUI->SetItemList(wName);
			listUI->SetEvent(this, std::bind(&GUIMeshRenderer::SetModel
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

	void GUIMeshRenderer::SetModel(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(wKey);

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		GameObj* target = inspector->GetTargetGameObject();
		MeshRenderer* mr = target->GetComponent<MeshRenderer>();
		Material* mt = model->GetMaterial();

		if (mt == nullptr)
			mt = GETSINGLE(ResourceMgr)->Find<Material>(L"PhongMaterial");

		mr->SetMaterial(mt);
		mr->SetModel(model, mt);
	}
}
