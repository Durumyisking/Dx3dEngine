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
		//ImGui::SetNextWindowCollapsed(false);  // �����츦 ������ ���� ���·� �ʱ�ȭ
		//ImGui::SetNextWindowFocus();  // �����쿡 ��Ŀ�� ����
		//ImGui::SetNextWindowContentSize(ImVec2(10, 10));  // ������ ũ�� �ʱ�ȭ
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


		//// ��ũ�� ��ǥ�� NDC ��ǥ�� ��ȯ
		//float ndcX = (2.0f * mouseX / screenWidth) - 1.0f;
		//float ndcY = 1.0f - (2.0f * mouseY / screenHeight);

		//// NDC ��ǥ�� ���� ��ǥ�� ��ȯ
		//XMMATRIX invViewProjection = XMMatrixInverse(nullptr, viewProjectionMatrix);
		//XMVECTOR worldPosition = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 0.0f, 1.0f), invViewProjection);
		//XMFLOAT3 newPosition;
		//XMStoreFloat3(&newPosition, worldPosition);

		//// ��ü ��ġ ������Ʈ
		//objectPosition = newPosition;
	}

}
