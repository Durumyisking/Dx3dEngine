#include "guiHierarchy.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneMgr.h"
#include "guiInspector.h"
#include "guiEditor.h"
#include "Renderer.h"
#include "guiInspector.h"
#include "Application.h"

extern dru::Application application;
extern gui::Editor editor;

namespace gui
{
	Hierarchy::Hierarchy()
		: mTreeWidget(nullptr)
	{
		SetName("Hierarchy");
		SetSize(ImVec2(1600 / 2, 900 / 2));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Scenes");
		AddWidget(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&Hierarchy::InitializeInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeScene();
	}

	Hierarchy::~Hierarchy()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}

	void Hierarchy::FixedUpdate()
	{
	}

	void Hierarchy::Update()
	{
	}

	void Hierarchy::LateUpdate()
	{
	}

	void Hierarchy::InitializeInspector(void* data)
	{
		dru::renderer::inspectorGameObject 
			= static_cast<dru::GameObj*>(data);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(dru::renderer::inspectorGameObject);
		inspector->InitializeTargetGameObject();


	}

	void Hierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		dru::Scene* scene = dru::SceneMgr::mActiveScene;
		std::string sceneName(scene->GetName().begin(), scene->GetName().end());

		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);

		for (size_t i = 0; i < (UINT)dru::enums::eLayerType::End; i++)
		{
			dru::Layer& layer = scene->GetLayer((dru::enums::eLayerType)i);
			const std::vector<dru::GameObj*>& gameObjs
				= layer.GetGameObjects();

			for (dru::GameObj* obj : gameObjs)
			{
				AddGameObject(root, obj);
			}
		}

	}

	void Hierarchy::AddGameObject(TreeWidget::Node* parent, dru::GameObj* gameObject)
	{
		std::string name(gameObject->GetName().begin(), gameObject->GetName().end());

		TreeWidget::Node* node = mTreeWidget->AddNode(parent, name, gameObject);
	}

}
