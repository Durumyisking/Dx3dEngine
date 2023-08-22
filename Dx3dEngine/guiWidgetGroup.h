#pragma once
#include "guiWidget.h"

namespace gui
{
    class WidgetGroup :
        public Widget
    {
    public:
		WidgetGroup(std::string name);
		virtual ~WidgetGroup();

		virtual void Update() override;
		void Close() override;

		void BeginGroup();
		void EndGroup();

		void SetEvent(Widget* widget, std::function<void(std::string key)> event)
		{
			mEventWidget = widget;
			mEvent = std::move(event);
		}

		void CollapseGroupHeader(std::string headerName, bool open = true);

		void AddGroupList(const std::vector<std::wstring>& list, Widget* widget);
		void AddGroupList(const std::string name, Widget* widget);

	private:
		std::map<std::string, Widget*> mGroupList;

		Widget* mEventWidget;
		std::function<void(std::string key)> mEvent;
    };
}

