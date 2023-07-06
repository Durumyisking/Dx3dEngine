#include "guiTransform.h"
#include "Transform.h"

namespace gui
{
	Transform::Transform()
		: Component(eComponentType::Transform)
	{
		SetName("Transform");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	Transform::~Transform()
	{

	}

	void Transform::FixedUpdate()
	{
		Component::FixedUpdate();


		if (GetTarget() == nullptr)
			return;

		dru::Transform* tr = GetTarget()->GetComponent<dru::Transform>();

		mPosisition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
	}

	void Transform::Update()
	{
		Component::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosisition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			dru::Transform* tr = GetTarget()->GetComponent<dru::Transform>();

			tr->SetPosition(mPosisition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}

	void Transform::LateUpdate()
	{
		Component::LateUpdate();

	}
}
