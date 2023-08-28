#pragma once
#include "guiWidget.h"

namespace gui
{
    class GroupWidget :
        public Widget
    {
    public:
		GroupWidget();
		GroupWidget(std::string name);
		virtual ~GroupWidget();


		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;


		void SetCollpase(bool enable, bool open = true)
		{
			mbCollapse = enable;
			mbGroupOpen = open;
		}

		template<typename T>
		T* CreateWidget(const std::string& name, float width = 100.f, float height = 100.f)
		{
			T* Widget = new T;

			Widget->SetName(GetName().c_str() + '_' + name);
			Widget->SetParent(this);
			Widget->SetSize(width, height);

			if (!Widget->Initialize())
			{
				if (Widget)
				{
					delete Widget;
					Widget = nullptr;
				}
				return nullptr;
			}

			mChilds.push_back(Widget);

			return Widget;
		}

	private:
		bool mbCollapse;
		bool mbGroupOpen;
    };
}

