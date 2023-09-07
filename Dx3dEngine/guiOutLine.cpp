#include "guiOutLine.h"

#include "Animator.h"
#include "BoneAnimator.h"
#include "MeshRenderer.h"


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
	OutLine::OutLine()
		: Widget()
		, mTreeWidget(nullptr)
		, mTargetGameObject(nullptr)
		, mTargetResource(nullptr)
		, mComponents{}
		, mResources{}
	{
		SetName("OutLine");
		mComponents.resize(static_cast<UINT>(eComponentType::End));
		mResources.resize(static_cast<UINT>(eResourceType::End));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Resources");
		AddWidget(mTreeWidget);
	}
	OutLine::~OutLine()
	{
		for (gui::GUIComponent* comp : mComponents)
		{
			delete comp;
			comp = nullptr;
		}

		for (gui::GUIResource* res : mResources)
		{
			delete res;
			res = nullptr;
		}
	}

	void OutLine::FixedUpdate()
	{
	}
	void OutLine::Update()
	{
	}
	void OutLine::LateUpdate()
	{
	}

	void OutLine::ResetContent()
	{	//mTreeWidget->Close();
		mTreeWidget->Clear();

		TreeWidget::Node* pRootNode = mTreeWidget->AddNode(nullptr, "Resources", 0, true);

		//enum class eResourceType
		//{
		//	GUIMesh,
		//	GUITexture,
		//	GUIMaterial,
		//	Sound,
		//	Prefab,
		//	MeshData,
		//	GraphicsShader,
		//	ComputeShader,
		//	End,
		//};
		AddResources<Mesh>(pRootNode, "Mesh");
		AddResources<Texture>(pRootNode, "Texture");
		AddResources<Material>(pRootNode, "Materials");
		AddResources<Shader>(pRootNode, "Shaders");
	}

	void OutLine::ClearTarget()
	{
		mTargetGameObject = nullptr;
		mTargetResource = nullptr;
		mComponents.clear();
		mResources.clear();
	}


	void OutLine::InitializeTargetGameObject()
	{
		ClearTarget();
		
		for (size_t i = 0; i < static_cast<UINT>(eComponentType::End); i++)
		{

		}
	}
	void OutLine::InitializeTargetResource()
	{
	}

	bool OutLine::GetComponentType(eComponentType type)
	{
		switch (type)
		{
		case enums::eComponentType::None:
			return false;
		case enums::eComponentType::Transform:
			if (mTargetGameObject->GetComponent<Transform>())
				return true;
		case enums::eComponentType::Camera:
			if (mTargetGameObject->GetComponent<Camera>())
				return true;
		case enums::eComponentType::Mesh:
			if (mTargetGameObject->GetComponent<Mesh>())
				return true;
		case enums::eComponentType::RigidBody:
			if (mTargetGameObject->GetComponent<PhysXRigidBody>())
				return true;
		case enums::eComponentType::Movement:
			if (mTargetGameObject->GetComponent<PhysicalMovement>())
				return true;
		case enums::eComponentType::Physical:
			if (mTargetGameObject->GetComponent<Physical>())
				return true;
		case enums::eComponentType::Collider:
			if (mTargetGameObject->GetComponent<PhysXCollider>())
				return true;
		case enums::eComponentType::Animator:
			if (mTargetGameObject->GetComponent<Animator>())
				return true;
		case enums::eComponentType::BoneAnimator:
			if (mTargetGameObject->GetComponent<BoneAnimator>())
				return true;
		case enums::eComponentType::MeshRenderer:
			if (mTargetGameObject->GetComponent<MeshRenderer>())
				return true;
		case enums::eComponentType::Particle:
			if (mTargetGameObject->GetComponent<Particle>())
				return true;
		case enums::eComponentType::Light:
			if (mTargetGameObject->GetComponent<Light>())
				return true;
		case enums::eComponentType::UI:
				return false;
		case enums::eComponentType::AudioListener:
			if (mTargetGameObject->GetComponent<AudioListener>())
				return true;
		case enums::eComponentType::AudioSource:
			if (mTargetGameObject->GetComponent<AudioSource>())
				return true;
		case enums::eComponentType::Script:
			return false;
		case enums::eComponentType::End:
			return false;
		default:
			return false;
		}
	}
	void OutLine::toInspector(void* data)
	{
		Resource* resource = static_cast<Resource*>(data);

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->SetTargetResource(resource);
		inspector->InitializeTargetResource();
	}
}