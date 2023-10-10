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
		, mObjectName()
		, mInputText()
		, mInputLayer(eLayerType::None)
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
		mGroupWidget->SetName("ObjectWindowGroup");
		mGroupWidget->SetSpacing(10.f, 5.f);
		mGroupWidget->SetNextLine(2);
		AddWidget(mGroupWidget);


		ListWidget* CDOList = mGroupWidget->CreateWidget<ListWidget>(150.f, 300.f);
		CDOList->SetState(eState::Active);
		CDOList->SetAllOpen(true);

		std::vector<std::string> CDOTypeVec;
		GameObj::GetAllCDOType(&CDOTypeVec);

		CDOList->SetItemList(CDOTypeVec);
		CDOList->SetEvent(this, std::bind(&ObjectWindow::GetObjectCDO, this, std::placeholders::_1));


		ButtonWidget* button = mGroupWidget->CreateWidget<ButtonWidget>(110.f, 70.f);
		button->SetName("CreateObject");
		button->SetClickCallback(&ObjectWindow::CreateObject, this);

		//
	}

	void ObjectWindow::FixedUpdate()
	{
	}

	void ObjectWindow::Update()
	{
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

		if(!mObjectName.empty())
			NewObj->SetName(mObjectName);

		mInputLayer = eLayerType::Objects;

		if (AddObjectToScene(NewObj, mInputLayer))
		{
			//Hierachy °»½Å;
			Hierarchy* hierarchy = GETSINGLE(WidgetMgr)->GetWidget<Hierarchy>("Hierarchy");
			hierarchy->InitializeScene();
		}
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