#include "guiHierarchy.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneMgr.h"
#include "guiInspector.h"
#include "guiEditor.h"
#include "Renderer.h"
#include "guiInspector.h"
#include "Application.h"

#include "guiTreeWidget.h"
#include "guiWidgetMgr.h"
#include "guiOutLine.h"

#include "InputMgr.h"

extern Application application;
extern gui::Editor editor;

namespace gui
{
	Hierarchy::Hierarchy()
		: mTreeWidget(nullptr)
		, mTargetObject(nullptr)
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
		//ImVec2 defaultInit = ImVec2(100.f, 100.f);
		//ImGui::SetNextWindowPos(defaultInit);
		//ImGui::SetNextWindowSize(defaultInit);
		//ImGui::SetNextWindowCollapsed(false);  // 윈도우를 접히지 않은 상태로 초기화
		//ImGui::SetNextWindowFocus();  // 윈도우에 포커스 설정
		//ImGui::SetNextWindowContentSize(ImVec2(10, 10));  // 컨텐츠 크기 초기화
	}

	void Hierarchy::Update()
	{
		if (KEY_DOWN(LCTRL))
		{
			if (KEY_TAP(LBTN))
			{
				MoveTarget();
			}
		}
	}

	void Hierarchy::LateUpdate()
	{
	}

	void Hierarchy::InitializeInspector(void* data)
	{
		mTargetObject = static_cast<GameObj*>(data);

		renderer::inspectorGameObject 
			= static_cast<GameObj*>(data);

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(renderer::inspectorGameObject);
		inspector->InitializeTargetGameObject();

		OutLine* outline = GETSINGLE(WidgetMgr)->GetWidget<OutLine>("Outline");
		if (outline == nullptr)
			return;
		outline->SetTargetGameObject(renderer::inspectorGameObject);
		outline->InitializeTargetGameObject();
	}

	void Hierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		std::string sceneName(scene->GetName().begin(), scene->GetName().end());

		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);

		for (size_t i = 0; i < static_cast<UINT>(enums::eLayerType::End); i++)
		{
			Layer& layer = scene->GetLayer((enums::eLayerType)i);
			const std::vector<GameObj*>& gameObjs
				= layer.GetGameObjects();

			for (GameObj* obj : gameObjs)
			{
				AddGameObject(root, obj);
			}
		}

	}

	void Hierarchy::AddGameObject(TreeWidget::Node* parent, GameObj* gameObject)
	{
		std::string name(gameObject->GetName().begin(), gameObject->GetName().end());

		TreeWidget::Node* node = mTreeWidget->AddNode(parent, name, gameObject);
	}

	void Hierarchy::MoveTarget()
	{
		//POINT clientPoint;
		//clientPoint.x = mouseX;
		//clientPoint.y = mouseY;
		//ScreenToClient(hwnd, &clientPoint);

		//float clientX = static_cast<float>(clientPoint.x);
		//float clientY = static_cast<float>(clientPoint.y);


		//// 스크린 좌표를 NDC 좌표로 변환
		//float ndcX = (2.0f * mouseX / screenWidth) - 1.0f;
		//float ndcY = 1.0f - (2.0f * mouseY / screenHeight);

		//// NDC 좌표를 월드 좌표로 변환
		//XMMATRIX invViewProjection = XMMatrixInverse(nullptr, viewProjectionMatrix);
		//XMVECTOR worldPosition = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 0.0f, 1.0f), invViewProjection);
		//XMFLOAT3 newPosition;
		//XMStoreFloat3(&newPosition, worldPosition);

		//// 물체 위치 업데이트
		//objectPosition = newPosition;
	}

}
