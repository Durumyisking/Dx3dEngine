#include "guiTransform.h"
#include "Transform.h"
#include "ImGuizmo.h"
#include "guiWidgetMgr.h"
#include "guiGizmo.h"
#include <charconv>
#include "Physical.h"
#include "guiInspector.h"


namespace gui
{
	GUITransform::GUITransform()
		: GUIComponent(eComponentType::Transform)
		, mPosition()
		, mRotation()
		, mScale()
	{
		SetName("Transform");
		SetSize(ImVec2(285.0f, 100.0f));
	}

	GUITransform::~GUITransform()
	{

	}

	void GUITransform::FixedUpdate()
	{
		GUIComponent::FixedUpdate();


		if (GetTarget() == nullptr)
			return;

		Transform* tr = GetTarget()->GetComponent<Transform>();

		if (tr == nullptr)
			return;

		mPosition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
	}

	void GUITransform::Update()
	{
		GUIComponent::Update();


		bool posChanged = false;
		bool rotChanged = false;
		bool scaleChanged = false;

		/*float pos[3] = { mPosition.x, mPosition.y, mPosition.z };
		float rot[3] = { mRotation.x, mRotation.y, mRotation.z };
		float scale[3] = { mScale.x, mScale.y, mScale.z };

		ImGui::Text("Position"); ImGui::SameLine();
		if (ImGui::InputFloat3("##Position", pos)) {
			mPosition = { pos[0], pos[1], pos[2] };
		}

		ImGui::Text("Rotation"); ImGui::SameLine();
		if (ImGui::InputFloat3("##Rotation", rot)) {
			mRotation = { rot[0], rot[1], rot[2] };
		}

		ImGui::Text("Scale"); ImGui::SameLine();
		if (ImGui::InputFloat3("##Scale", scale)) {
			mScale = { scale[0], scale[1], scale[2] };
		}*/

		posChanged = ImGui::InputFloat3("#Position", (float*)&mPosition);

		rotChanged = ImGui::InputFloat3("#Rotation", (float*)&mRotation);

		scaleChanged = ImGui::InputFloat3("#Scale", (float*)&mScale);

		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("AddPhysical", ImVec2(70.f, 20.f)))
		{
			GameObj* targetObj = GetTarget();

			if (targetObj != nullptr)
			{
				if (targetObj->GetComponent<Physical>() == nullptr)
				{
					Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");

					inspector->AddPhysical();
				}
			}
		}

		if (!(posChanged || rotChanged || scaleChanged))
			return;

		if (GetTarget())
		{
			Transform* tr = GetTarget()->GetComponent<Transform>();

			if (tr == nullptr)
				return;

			if (GetTarget()->GetComponent<Physical>() != nullptr)
			{
				tr->SetPhysicalPosition(mPosition);
				tr->SetPhysicalRotation(mRotation);
				tr->SetScale(mScale);
			}
			else
			{
				tr->SetPosition(mPosition);
				tr->SetRotation(mRotation);
				tr->SetScale(mScale);
			}
		}
	}

	void GUITransform::LateUpdate()
	{
		GUIComponent::LateUpdate();

	}

	void GUITransform::InputFloatValues(math::Vector3& value) {
		const char* labels[3] = { "X", "Y", "Z" };
		float* components[3] = { &value.x, &value.y, &value.z };
		char inputBuffers[3][32];

		// 초기 값 설정
		for (int i = 0; i < 3; ++i) 
		{
			snprintf(inputBuffers[i], sizeof(inputBuffers[i]), "%f", *components[i]);
		}

		for (int i = 0; i < 3; ++i) 
		{
			ImGui::PushItemWidth(50.0f);

			bool editFinished = ImGui::InputText(labels[i], inputBuffers[i], sizeof(inputBuffers[i]), ImGuiInputTextFlags_EnterReturnsTrue);

			if (editFinished || (ImGui::IsItemDeactivatedAfterEdit() && ImGui::IsItemFocused() == false)) {
				float newValue;
				auto result = std::from_chars(inputBuffers[i], inputBuffers[i] + std::strlen(inputBuffers[i]), newValue);

				if (result.ec == std::errc()) {
					*components[i] = newValue;
				}
			}
			ImGui::PopItemWidth();
		}
	}
}
