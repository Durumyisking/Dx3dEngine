#include "guiInspector.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Renderer.h"

#include "guiTransform.h"
#include "guiMeshRenderer.h"
#include "guiTexture.h"

namespace gui
{
	using namespace dru::enums;
	Inspector::Inspector()
		:mTargetResource(nullptr)
	{
		SetName("Inspector");
		SetSize(ImVec2(300.0f, 100.0f));
		
		mComponents.resize(static_cast<UINT>(eComponentType::End));
		mTargetGameObject = dru::renderer::inspectorGameObject;

		mComponents[static_cast<UINT>(eComponentType::Transform)] = new gui::Transform();
		mComponents[static_cast<UINT>(eComponentType::Transform)]->SetName("InspectorTransform");
		mComponents[static_cast<UINT>(eComponentType::Transform)]->SetTarget(mTargetGameObject);
		AddWidget(mComponents[static_cast<UINT>(eComponentType::Transform)]);
		
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)] = new gui::MeshRenderer();
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetName("InspectorMeshRenderer");
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetTarget(mTargetGameObject);
		AddWidget(mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]);

		mResources.resize(static_cast<UINT>(eResourceType::End));
		mResources[static_cast<UINT>(eResourceType::Texture)] = new gui::Texture();
		mResources[static_cast<UINT>(eResourceType::Texture)]->SetName("InspectorTexture");
		AddWidget(mResources[static_cast<UINT>(eResourceType::Texture)]);
	}

	Inspector::~Inspector()
	{
		for (gui::Component* comp : mComponents)
		{
			delete comp;
			comp = nullptr;
		}

		for (gui::Resource* res : mResources)
		{
			delete res;
			res = nullptr;
		}
	}

	void Inspector::FixedUpdate()
	{
		//mComponents[static_cast<UINT>(eComponentType::Transform]->SetTarget(mTarget);
		//mComponents[static_cast<UINT>(eComponentType::MeshRenderer]->SetTarget(mTarget);
	}

	void Inspector::Update()
	{
		
	}

	void Inspector::LateUpdate()
	{
	}

	void Inspector::ClearTarget()
	{
		for (gui::Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->SetState(eState::Paused);
			comp->SetTarget(nullptr);
		}

		for (gui::Resource* res : mResources)
		{
			if (res == nullptr)
				continue;

			res->SetState(eState::Paused);
			res->SetTarget(nullptr);
		}
	}

	void Inspector::InitializeTargetGameObject()
	{
		ClearTarget();

		mComponents[static_cast<UINT>(eComponentType::Transform)]->SetState(eState::Active);
		mComponents[static_cast<UINT>(eComponentType::Transform)]->SetTarget(mTargetGameObject);
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetState(eState::Active);
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetTarget(mTargetGameObject);
	}

	void Inspector::InitializeTargetResource()
	{
		ClearTarget();

		mResources[static_cast<UINT>(eResourceType::Texture)]->SetState(eState::Active);
		mResources[static_cast<UINT>(eResourceType::Texture)]->SetTarget(mTargetResource);
	}
}