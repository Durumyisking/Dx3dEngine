#include "guiOutLiner.h"

#include "Animator.h"
#include "BoneAnimator.h"
#include "MeshRenderer.h"

#include "RigidBody.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"
#include "PhysicsMgr.h"
#include "PhysicsScene.h"
#include "PhysX.h"


#include "AudioListener.h"
#include "AudioSource.h"

#include "guiInspector.h"
#include "guiWidgetMgr.h"

namespace gui
{
	OutLiner::OutLiner()
		: Widget()
		, mTreeWidget(nullptr)
		, mTargetGameObject(nullptr)
		, mComponents{}
	{
		SetName("OutLine");

		mComponents.resize(static_cast<UINT>(enums::eComponentType::End));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("ObjectsComponents");
		AddWidget(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&OutLiner::toInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeGameObject();
	}
	OutLiner::~OutLiner()
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

		ClearTarget();
	}

	void OutLiner::FixedUpdate()
	{
	}
	void OutLiner::Update()
	{
	}
	void OutLiner::LateUpdate()
	{
	}

	void OutLiner::InitializeGameObject()
	{	

	}

	void OutLiner::ClearTarget()
	{
		mTargetGameObject = nullptr;
		mComponents.clear();
	}

	void OutLiner::InitializeTargetGameObject()
	{
		mTreeWidget->Clear();

		if (mTargetGameObject == nullptr)
			return;

		std::string objName(mTargetGameObject->GetName().begin(), mTargetGameObject->GetName().end());

		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, objName, 0, true);

		root->ChildsResize(static_cast<UINT>(enums::eComponentType::End));

		mComponents = mTargetGameObject->GetComponentsVec();

		for (size_t i = 0; i < static_cast<UINT>(enums::eComponentType::End); i++)
		{
			if (!mComponents[i])
				continue;

			std::string name = "eComponentType::";
			std::string componentTypeName = enums::charComponentType[static_cast<UINT>(i)];

			TreeWidget::Node* rootChild = mTreeWidget->AddNode(root, name + componentTypeName, 0, false);

			//std::string componentName(mComponents[i]->GetName().begin(), mComponents[i]->GetName().end());
			//if (componentName == "")
			//	componentName = "NoName_" + std::to_string(i);

			//mTreeWidget->AddNode(rootChild, componentName, mComponents[i]);
		}

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(mTargetGameObject);
		inspector->InitializeTargetGameObject();
	}


	void OutLiner::toInspector(void* data)
	{
		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(mTargetGameObject);
		//inspector->InitializeTargetResource();
	}
}