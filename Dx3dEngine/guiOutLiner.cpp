#include "guiOutLiner.h"

#include "Animator.h"
#include "BoneAnimator.h"
#include "MeshRenderer.h"#


#include "RigidBody.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PlayerScript.h"
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

	void SetComponentTypeName(enums::eComponentType type, std::string& name)
	{
		switch (type)
		{
		case enums::eComponentType::None:
			name = "eComponentType::None";
			break;
		case enums::eComponentType::Transform:
			name = "eComponentType::Transform";
			break;
		case enums::eComponentType::Camera:
			name = "eComponentType::Camera";
			break;
		case enums::eComponentType::Mesh:
			name = "eComponentType::Mesh";
			break;
		case enums::eComponentType::RigidBody:
			name = "eComponentType::RigidBody";
			break;
		case enums::eComponentType::Movement:
			name = "eComponentType::Movement";
			break;
		case enums::eComponentType::Physical:
			name = "eComponentType::Physical";
			break;
		case enums::eComponentType::Collider:
			name = "eComponentType::Collider";
			break;
		case enums::eComponentType::Animator:
			name = "eComponentType::Animator";
			break;
		case enums::eComponentType::BoneAnimator:
			name = "eComponentType::BoneAnimator";
			break;
		case enums::eComponentType::MeshRenderer:
			name = "eComponentType::MeshRenderer";
			break;
		case enums::eComponentType::Renderer:
			name = "eComponentType::Renderer";
			break;
		case enums::eComponentType::Particle:
			name = "eComponentType::Particle";
			break;
		case enums::eComponentType::Light:
			name = "eComponentType::Light";
			break;
		case enums::eComponentType::UI:
			name = "eComponentType::UI";
			break;
		case enums::eComponentType::AudioListener:
			name = "eComponentType::AudioListener";
			break;
		case enums::eComponentType::AudioSource:
			name = "eComponentType::AudioSource";
			break;
		case enums::eComponentType::Script:
			name = "eComponentType::Script";
			break;
		}
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
			if (mComponents[i])
			{
				std::string componentTypeName = {};
				
				SetComponentTypeName(static_cast<enums::eComponentType>(i), componentTypeName);

				TreeWidget::Node* rootChild = mTreeWidget->AddNode(root, componentTypeName, 0, false);

				//std::string componentName(mComponents[i]->GetName().begin(), mComponents[i]->GetName().end());
				//if (componentName == "")
				//	componentName = "NoName_" + std::to_string(i);

				//mTreeWidget->AddNode(rootChild, componentName, mComponents[i]);
			}
		}
	}


	void OutLiner::toInspector(void* data)
	{
		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(mTargetGameObject);
		//inspector->InitializeTargetResource();
	}
}