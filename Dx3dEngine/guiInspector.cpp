#include "guiInspector.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Renderer.h"
#include "Component.h"
#include "MeshRenderer.h"

#include "guiWidgetMgr.h"
#include "guiHierarchy.h"
#include "guiComponent.h"
#include "guiResource.h"

#include "Resource.h"

namespace gui
{
	using namespace enums;
	Inspector::Inspector()
		:mTargetResource(nullptr)
		, mTransform(nullptr)
		, mMeshRenderer(nullptr)
	{
		SetName("Inspector");
		SetSize(ImVec2(300.0f, 100.0f));
		
		mComponents.resize(static_cast<UINT>(eComponentType::End));
		mTargetGameObject = GETSINGLE(WidgetMgr)->GetWidget<Hierarchy>("Hierarchy")->GetTargetObject();

		mTransform = new gui::GUITransform();
		mComponents[static_cast<UINT>(eComponentType::Transform)] = mTransform;
		mComponents[static_cast<UINT>(eComponentType::Transform)]->SetName("InspectorTransform");
		mComponents[static_cast<UINT>(eComponentType::Transform)]->SetTarget(mTargetGameObject);
		AddWidget(mComponents[static_cast<UINT>(eComponentType::Transform)]);

		mComponents[static_cast<UINT>(eComponentType::Physical)] = new gui::GUIPhysical();
		mComponents[static_cast<UINT>(eComponentType::Physical)]->SetName("InspectorPhysical");
		mComponents[static_cast<UINT>(eComponentType::Physical)]->SetTarget(mTargetGameObject);
		AddWidget(mComponents[static_cast<UINT>(eComponentType::Physical)]);

		mMeshRenderer = new gui::GUIMeshRenderer();
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)] = mMeshRenderer;
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetName("InspectorMeshRenderer");
		mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetTarget(mTargetGameObject);
		AddWidget(mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]);

		mResources.resize(static_cast<UINT>(eResourceType::End));
		mResources[static_cast<UINT>(eResourceType::Texture)] = new gui::GUITexture();
		mResources[static_cast<UINT>(eResourceType::Texture)]->SetName("InspectorTexture");
		AddWidget(mResources[static_cast<UINT>(eResourceType::Texture)]);

		//bool firstFrame = true;
	}

	Inspector::~Inspector()
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

	void Inspector::FixedUpdate()
	{
		//mComponents[static_cast<UINT>(eComponentType::GUITransform]->SetTarget(mTarget);
		//mComponents[static_cast<UINT>(eComponentType::GUIMeshRenderer]->SetTarget(mTarget);
	}

	void Inspector::Update()
	{
		//Camera Edit
		/*ImGui::Text("Camera");
		bool viewDirty = false;

		if (ImGui::RadioButton("Perspective", isPerspective)) isPerspective = true;
		ImGui::SameLine();
		if (ImGui::RadioButton("Orthographic", !isPerspective)) isPerspective = false;
		if (isPerspective)
		{
			ImGui::SliderFloat("Fov", &fov, 20.f, 110.f);
		}
		else
		{
			ImGui::SliderFloat("Ortho width", &viewWidth, 1, 20);
		}
		viewDirty |= ImGui::SliderFloat("Distance", &camDistance, 1.f, 10.f);
		ImGui::SliderInt("Gizmo count", &gizmoCount, 1, 4);

		if (viewDirty || firstFrame)
		{
			float eye[] = { cosf(camYAngle) * cosf(camXAngle) * camDistance, sinf(camXAngle) * camDistance, sinf(camYAngle) * cosf(camXAngle) * camDistance };
			float at[] = { 0.f, 0.f, 0.f };
			float up[] = { 0.f, 1.f, 0.f };
			LookAt(eye, at, up, cameraView);
			firstFrame = false;
		}



		ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
		if (ImGuizmo::IsUsing())
		{
			ImGui::Text("Using gizmo");
		}
		else
		{
			ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
		}*/

		//ImGui::Separator();
		
		//ImGuizmo::SetID(matId);
		//EditTransform(cameraView, cameraProjection, objectMatrix[matId], lastUsing == matId);
	}

	void Inspector::LateUpdate()
	{
	}

	void Inspector::Render()
	{
		if (mState != eState::Active)
			return;

		bool open = (bool)GetState();
		FixedUpdate();
		if (!ImGui::Begin(GetName().c_str(), &open, mWindow_flags))
			mState = eState::Paused;
		Update();
		for (Widget* child : mChilds)
		{
			child->Render();

			ImGui::Separator();
		}
		LateUpdate();


		ImGui::End();
	}

	void Inspector::ClearTarget()
	{
		for (gui::GUIComponent* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->SetState(eState::Paused);
			comp->SetTarget(nullptr);
		}

		for (gui::GUIResource* res : mResources)
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

		std::vector<Component*> targetComponents = mTargetGameObject->GetComponentsVec();

		for (size_t i = 0; i < static_cast<UINT>(enums::eComponentType::End); i++)
		{
			if (!targetComponents[i])
				continue;

			if (i == static_cast<UINT>(eComponentType::Transform))
			{
				mComponents[static_cast<UINT>(eComponentType::Transform)]->SetState(eState::Active);
				mComponents[static_cast<UINT>(eComponentType::Transform)]->SetTarget(mTargetGameObject);
				mTransform->IsPhysical(false);
			}
			else if (i == static_cast<UINT>(eComponentType::Physical))
			{
				mComponents[static_cast<UINT>(eComponentType::Physical)]->SetState(eState::Active);
				mComponents[static_cast<UINT>(eComponentType::Physical)]->SetTarget(mTargetGameObject);
				mTransform->IsPhysical(true);
			}
			else if (i == static_cast<UINT>(eComponentType::MeshRenderer))
			{
				mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetState(eState::Active);
				mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]->SetTarget(mTargetGameObject);
			}
		}
	}

	void Inspector::InitializeTargetResource()
	{
		ClearTarget();

		mResources[static_cast<UINT>(eResourceType::Texture)]->SetState(eState::Active);
		mResources[static_cast<UINT>(eResourceType::Texture)]->SetTarget(mTargetResource);
	}
}