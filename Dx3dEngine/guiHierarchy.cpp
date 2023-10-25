#include "guiHierarchy.h"
#include "Layer.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "guiInspector.h"
#include "guiEditor.h"
#include "Renderer.h"
#include "guiInspector.h"
#include "Application.h"

#include "guiTreeWidget.h"
#include "guiWidgetMgr.h"
#include "guiOutLiner.h"

#include "InputMgr.h"
#include "PhysXRayCast.h"

extern Application application;
extern gui::Editor editor;

namespace gui
{
	Hierarchy::Hierarchy()
		: mbForceReset(false)
		, mTreeWidget(nullptr)
		, mTargetObject(nullptr)
		, mCurrentScene(nullptr)
	{
		SetName("Hierarchy");
		SetSize(ImVec2(1600 / 2, 900 / 2));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Scenes");
		AddWidget(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&Hierarchy::InitializeOutline, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		//InitializeScene();
	}

	Hierarchy::~Hierarchy()
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

	void Hierarchy::FixedUpdate()
	{
		//ImVec2 defaultInit = ImVec2(100.f, 100.f);
		//ImGui::SetNextWindowPos(defaultInit);G
		//ImGui::SetNextWindowSize(defaultInit);
		//ImGui::SetNextWindowCollapsed(false);  // 윈도우를 접히지 않은 상태로 초기화
		//ImGui::SetNextWindowFocus();  // 윈도우에 포커스 설정
		//ImGui::SetNextWindowContentSize(ImVec2(10, 10));  // 컨텐츠 크기 초기화


		Scene* mActiveScene = GETSINGLE(SceneMgr)->GetActiveScene();
		if (mCurrentScene != mActiveScene || mbForceReset)
		{
			if (mActiveScene != nullptr)
			{
				mCurrentScene = mActiveScene;

				mTargetObject = nullptr;
				InitializeScene();
				mbForceReset = false;

				InitializeOutline(mTargetObject);
			}
		}
	}

	void Hierarchy::Update()
	{

	}

	void Hierarchy::LateUpdate()
	{
	}

	void Hierarchy::InitializeOutline(void* data)
	{
		mTargetObject = static_cast<GameObj*>(data);

		OutLiner* outline = GETSINGLE(WidgetMgr)->GetWidget<OutLiner>("OutLiner");

		if (outline == nullptr)
			return;

		outline->ClearTarget();
		outline->SetTargetGameObject(mTargetObject);
		outline->InitializeTargetGameObject();
	}


	void Hierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		//Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		std::string sceneName(mCurrentScene->GetName().begin(), mCurrentScene->GetName().end());

		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);


		for (int i = 0; i < static_cast<int>(enums::eLayerType::End); i++)
		{
			Layer& layer = mCurrentScene->GetLayer((enums::eLayerType)i);
			const std::vector<GameObj*>& gameObjs = layer.GetGameObjects();

			std::string name = "eLayerType::";
			std::string layerTypeName = enums::charLayerType[i];

			TreeWidget::Node* rootChild = mTreeWidget->AddNode(root, name + layerTypeName, 0, false);

			for (GameObj* obj : gameObjs)
			{
				AddGameObject(rootChild, obj);
			}
		}

		InitializeOutline(mTargetObject);
	}

	void Hierarchy::AddGameObject(TreeWidget::Node* parent, GameObj* gameObject)
	{
		std::string name(gameObject->GetName().begin(), gameObject->GetName().end());

		if (name.empty())
		{
			name = "no name";
		}

		TreeWidget::Node* node = mTreeWidget->AddNode(parent, name, gameObject);
	}

}
