#include "guiComponent.h"
#include "GameObj.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
	GUIComponent::GUIComponent(enums::eComponentType type)
		: mType(type)
		, mTarget(nullptr)
	{


	}

	GUIComponent::~GUIComponent()
	{

	}

	void GUIComponent::FixedUpdate()
	{

	}

	void GUIComponent::Update()
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		ImGui::Button(enums::charComponentType[static_cast<UINT>(mType)]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	void GUIComponent::LateUpdate()
	{

	}
}
