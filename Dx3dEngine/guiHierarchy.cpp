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
		: mTreeWidget(nullptr)
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
		if (mCurrentScene != mActiveScene)
		{
			if (mActiveScene != nullptr)
			{
				mCurrentScene = mActiveScene;

				InitializeScene();
			}
		}


		if (KEY_UP(LSHIFT))
			GETSINGLE(PhysXRayCast)->ReleaseRaycast();

		if (!KEY_DOWN(LSHIFT))
			return;

		if (KEY_TAP(LBTN))
		{
			mTargetObject = GETSINGLE(PhysXRayCast)->Raycast();

			InitializeOutline(mTargetObject);
		}

		if (KEY_DOWN(LBTN))
		{
			GETSINGLE(PhysXRayCast)->MoveObject();
		}

		if (KEY_UP(LBTN))
		{
			GETSINGLE(PhysXRayCast)->ReleaseRaycast();
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

		renderer::outlineGameObject
			= static_cast<GameObj*>(data);

		OutLiner* outline = GETSINGLE(WidgetMgr)->GetWidget<OutLiner>("OutLiner");
		if (outline == nullptr)
			return;
		outline->ClearTarget();
		outline->SetTargetGameObject(renderer::outlineGameObject);
		outline->InitializeTargetGameObject();

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(renderer::outlineGameObject);
		inspector->InitializeTargetGameObject();
	}

	void SetLayerTypeName(enums::eLayerType type, std::string& name)
	{
		switch (type)
		{
		case enums::eLayerType::None:
			name = "eLayerType::None";
			break;
		case enums::eLayerType::Camera:
			name = "eLayerType::Camera";
			break;
		case enums::eLayerType::Grid:
			name = "eLayerType::Grid";
			break;
		case enums::eLayerType::Objects:
			name = "eLayerType::Objects";
			break;
		case enums::eLayerType::PhysicalObject:
			name = "eLayerType::PhysicalObject";
			break;
		case enums::eLayerType::Monster:
			name = "eLayerType::Monster";
			break;
		case enums::eLayerType::Boss:
			name = "eLayerType::Boss";
			break;
		case enums::eLayerType::Player:
			name = "eLayerType::Player";
			break;
		case enums::eLayerType::AfterImage:
			name = "eLayerType::AfterImage";
			break;
		case enums::eLayerType::Platforms:
			name = "eLayerType::Platforms";
			break;
		case enums::eLayerType::Particle:
			name = "eLayerType::Particle";
			break;
		case enums::eLayerType::FX:
			name = "eLayerType::FX";
			break;
		case enums::eLayerType::UI:
			name = "eLayerType::UI";
			break;
		case enums::eLayerType::PostProcess:
			name = "eLayerType::PostProcess";
			break;
		}
	}

	void Hierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		//Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		std::string sceneName(mCurrentScene->GetName().begin(), mCurrentScene->GetName().end());

		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);


		for (size_t i = 0; i < static_cast<UINT>(enums::eLayerType::End); i++)
		{
			Layer& layer = mCurrentScene->GetLayer((enums::eLayerType)i);
			const std::vector<GameObj*>& gameObjs = layer.GetGameObjects();

			std::string layerName = {};

			SetLayerTypeName(static_cast<enums::eLayerType>(i), layerName);

			TreeWidget::Node* rootChild = mTreeWidget->AddNode(root, layerName, 0, false);

			for (GameObj* obj : gameObjs)
			{
				AddGameObject(rootChild, obj);
			}
		}

	}

	void Hierarchy::AddGameObject(TreeWidget::Node* parent, GameObj* gameObject)
	{
		std::string name(gameObject->GetName().begin(), gameObject->GetName().end());

		TreeWidget::Node* node = mTreeWidget->AddNode(parent, name, gameObject);
	}

}
