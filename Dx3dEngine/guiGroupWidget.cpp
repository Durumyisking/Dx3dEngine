#include "guiGroupWidget.h"

namespace gui
{
	GroupWidget::GroupWidget()
		: Widget()
		, mbCollapse(false)
		, mbGroupOpen(false)
	{
	}
	GroupWidget::GroupWidget(std::string name)
		: Widget()
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
		if (mState != eState::Active)
			return;

		//for (Widget* child : mChilds)
		//{
		//	child->FixedUpdate();
		//}
	}

	void GroupWidget::Update()
	{
		if (mState != eState::Active)
			return;

		//for (Widget* child : mChilds)
		//{
		//	child->Update();
		//}
	}

	void GroupWidget::LateUpdate()
	{
		if (mState != eState::Active)
			return;

		//for (Widget* child : mChilds)
		//{
		//	child->LateUpdate();
		//}
	}

	void GroupWidget::Render()
	{
		ImGui::BeginGroup();

		if (mbCollapse)
		{
			if (ImGui::CollapsingHeader(GetName().c_str(), &mbGroupOpen))
			{
				for (size_t i = 0; i < mChilds.size(); ++i)
				{
					mChilds[i]->Render();
				}
			}
		}
		else
		{
			for (size_t i = 0; i < mChilds.size(); ++i)
			{
				mChilds[i]->Render();
			}
		}

		ImGui::EndGroup();
	}
}