#include "guiGroupWidget.h"

namespace gui
{
	GroupWidget::GroupWidget()
		: Widget()
		, mSpace{10.f, 0.f}
		, mNextLine(1)
		, mbCollapse(false)
		, mbGroupOpen(false)
	{
	}
	GroupWidget::GroupWidget(std::string name)
		: Widget()
		, mSpace(10.f, 0.f)
		, mNextLine(1)
		, mbCollapse(false)
		, mbGroupOpen(false)
	{
		SetName(name);
	}

	GroupWidget::~GroupWidget()
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
	}

	void GroupWidget::FixedUpdate()
	{
	}

	void GroupWidget::Update()
	{
	}

	void GroupWidget::LateUpdate()
	{
	}

	void GroupWidget::Render()
	{
		//if (mState != eState::Active)
		//	return;

		ImGui::BeginGroup();

		if (mbCollapse)
		{
			ImGui::CollapsingHeader(GetName().c_str(), &mbGroupOpen);
		}

		if (GetParent() == nullptr)
		{
			bool open = (bool)GetState();
			FixedUpdate();
			if (!ImGui::Begin(GetName().c_str(), &open, mWindow_flags | ImGuiWindowFlags_NoSavedSettings))
				mState = eState::Paused;
			Update();

			UINT count = 0;
			for (Widget* child : mChilds)
			{
				count++;

				child->Render();

				if (count % mNextLine == 0)
					continue;

				ImGui::SameLine();
				ImGui::Dummy(mSpace);
				//ImGui::Indent(float);
				ImGui::SameLine();
			}
			LateUpdate();

			ImGui::End();
		}
		else
		{
			FixedUpdate();
			ImGui::BeginChild(GetName().c_str(), mSize);
			//size 추가 해줘야한다.-
			Update();

			UINT count = 0;
			for (Widget* child : mChilds)
			{
				count++;

				child->Render();

				if (count % mNextLine == 0)
					continue;

				ImGui::SameLine();
				ImGui::Dummy(mSpace);
				//ImGui::Indent(float);
				ImGui::SameLine();
			}
			LateUpdate();
			//std::string debug = GetName();
			ImGui::EndChild();
		}

			//for (size_t i = 0; i < mChilds.size(); i++)
			//{
			//	mChilds[i]->Render();

			//	if ((i + 1) % mNextLine == 0)
			//		continue;

			//	ImGui::SameLine();
			//	ImGui::Dummy(mSpace);
			//	//ImGui::Indent(float);
			//	ImGui::SameLine();
			//}

		ImGui::EndGroup();
	}
	void GroupWidget::Clear()
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
	}
}