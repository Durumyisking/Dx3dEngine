#include "guiWidgetGroup.h"

namespace gui
{
	WidgetGroup::WidgetGroup(std::string name)
	{
		SetName(name);
		SetState(eState::Paused);
	}
	WidgetGroup::~WidgetGroup()
	{
	}

	void WidgetGroup::Update()
	{
		SetState(eState::Active);
	}
	void WidgetGroup::Close()
	{
		SetState(eState::Paused);
		mGroupList.clear();

		WindowFocus(false);
	}

	void WidgetGroup::BeginGroup()
	{
		ImGui::Begin(GetName().c_str());
	}
	void WidgetGroup::EndGroup()
	{
		ImGui::End();
	}

	void WidgetGroup::CollapseGroupHeader(std::string headerName, bool open)
	{
		ImGui::CollapsingHeader(headerName.c_str(), &open);
	}

	void WidgetGroup::AddGroupList(const std::vector<std::wstring>& list, Widget* widget)
	{
		for (auto wName : list)
		{
			std::string name(wName.begin(), wName.end());


			auto iter = mGroupList.find(name);
			if (iter == mGroupList.end())
			{
				mGroupList.insert(std::make_pair(name, widget));
			}
		}
	}
	void WidgetGroup::AddGroupList(const std::string name, Widget* widget)
	{
		auto iter = mGroupList.find(name);
		if (iter == mGroupList.end())
		{
			mGroupList.insert(std::make_pair(name, widget));
		}
	}
}