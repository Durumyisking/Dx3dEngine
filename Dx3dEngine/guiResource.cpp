#include "guiResource.h"


namespace gui
{
	GUIResource::GUIResource()
	{

	}

	GUIResource::~GUIResource()
	{

	}

	void GUIResource::FixedUpdate()
	{
		if (mTarget == nullptr)
			return;

		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		ImGui::Button(enums::charResourceType[static_cast<UINT>(mTarget->GetType())]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}