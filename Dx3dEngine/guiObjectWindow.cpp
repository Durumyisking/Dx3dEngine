#include "guiObjectWindow.h"
#include "SceneMgr.h"
#include "Scene.h"

#include "guiButtonWidget.h"
#include "guiListWidget.h"
#include "guiWidgetMgr.h"
#include "guiHierarchy.h"

namespace gui
{
	ObjectWindow::ObjectWindow()
		: Widget()
		, mCDOName()
		, mInputText()
		, mInputLayer(eLayerType::Objects)
	{
		SetName("ObjectWindow");
		SetSize(ImVec2(300.f, 300.f));
	}

	ObjectWindow::~ObjectWindow()
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

	void ObjectWindow::Initialize()
	{
		////GroupWidget Template
		mGroupWidget = new GroupWidget();
		mGroupWidget->SetName("ObjectWindowListGroup");
		mGroupWidget->SetSpacing(5.f, 5.f);
		mGroupWidget->SetNextLine(2);
		AddWidget(mGroupWidget);

		{
			ListWidget* CDOList = mGroupWidget->CreateWidget<ListWidget>(150.f, 300.f);
			CDOList->SetState(eState::Active);
			CDOList->SetAllOpen(true);

			std::vector<std::string> CDOTypeVec;
			GameObj::GetAllCDOType(&CDOTypeVec);

			CDOList->SetItemList(CDOTypeVec);
			CDOList->SetEvent(this, std::bind(&ObjectWindow::GetObjectCDO, this, std::placeholders::_1));
		}
		{
			TreeWidget* tree = mGroupWidget->CreateWidget<TreeWidget>(150.f, 200.f);
			tree->SetName("SelectObjectLayerType");

			tree->SetSimpleEvent(this
				, std::bind(&ObjectWindow::SetObjectLayerType, this, std::placeholders::_1));

			tree->SetDummyRoot(true);
			tree->Clear();

			TreeWidget::Node* root = tree->AddNode(nullptr, "SelectObjectLayerType", 0, true);

			for (size_t i = 0; i < static_cast<UINT>(enums::eLayerType::End); i++)
			{
				std::string layerTypeName = enums::charLayerType[static_cast<UINT>(i)];

				TreeWidget::Node* rootChild = tree->AddNode(root, layerTypeName, 0, false, i);
			}
		}
		{
			ButtonWidget* button = mGroupWidget->CreateWidget<ButtonWidget>(130.f, 75.f);
			button->SetName("CreateObject");
			button->SetClickCallback(&ObjectWindow::CreateObject, this);
		}
		{
			ButtonWidget* button1 = mGroupWidget->CreateWidget<ButtonWidget>(130.f, 75.f);
			button1->SetName("DeleteObject");
			button1->SetClickCallback(&ObjectWindow::DeleteObject, this);
		}

		//
	}

	void ObjectWindow::FixedUpdate()
	{
	}

	void ObjectWindow::Update()
	{
		ImGui::Text("Select Object Class  ");

		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();

		const int BUF_SIZE = 256;
		char buf[BUF_SIZE];

		ImGui::PushItemWidth(100.0f);
		
		// 초기화: std::string의 값을 버퍼에 복사
		strncpy_s(buf, mInputText.c_str(), sizeof(buf));
		buf[sizeof(buf) - 1] = 0; // 널 종료 문자를 보장

		if (ImGui::InputText("ObjectName", buf, sizeof(buf)))
			mInputText = buf;

		ImGui::PopItemWidth();
	}

	void ObjectWindow::LateUpdate()
	{
	}

	void ObjectWindow::CreateObject()
	{
		GameObj* objCDO = GameObj::FindObjectCDO(mCDOName);

		if (objCDO == nullptr)
			return;

		GameObj* NewObj = objCDO->Clone();

		if (NewObj == nullptr)
			return;

		if (!mInputText.empty())
			NewObj->SetName(StringToWStringReturn(mInputText));

		if (AddObjectToScene(NewObj, mInputLayer))
		{
			Hierarchy* hierarchy = GETSINGLE(WidgetMgr)->GetWidget<Hierarchy>("Hierarchy");
			hierarchy->ForceReset();
		}
	}

	void ObjectWindow::DeleteObject()
	{
		Hierarchy* hierarchy = GETSINGLE(WidgetMgr)->GetWidget<Hierarchy>("Hierarchy");

		if (hierarchy == nullptr)
			return;

		if (hierarchy->GetTargetObject() == nullptr)
			return;

		hierarchy->GetTargetObject()->Die();
		hierarchy->SetTargetObject(nullptr);
		hierarchy->ForceReset();
	}

	void ObjectWindow::SetObjectLayerType(UINT num)
	{
		mInputLayer = static_cast<eLayerType>(num);
	}

	bool ObjectWindow::AddObjectToScene(GameObj* obj, eLayerType type)
	{
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		
		if (scene == nullptr)
			return false;

		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(obj, type);
		layer.PushAddedObject(obj);

		return true;
	}

	void ObjectWindow::GetObjectCDO(const std::string& CDOTypeName)
	{
		mCDOName = CDOTypeName;
	}
}