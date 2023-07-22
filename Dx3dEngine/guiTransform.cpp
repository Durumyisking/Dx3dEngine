#include "guiTransform.h"
#include "Transform.h"

namespace gui
{
	GUITransform::GUITransform()
		: GUIComponent(eComponentType::Transform)
	{
		SetName("Transform");
		SetSize(ImVec2(200.0f, 120.0f));
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

		mPosisition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
	}

	void GUITransform::Update()
	{
		GUIComponent::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosisition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			Transform* tr = GetTarget()->GetComponent<Transform>();

			tr->SetPosition(mPosisition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}

	void GUITransform::LateUpdate()
	{
		GUIComponent::LateUpdate();

	}
}
