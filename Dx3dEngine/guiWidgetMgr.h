#pragma once
#include "Engine.h"
#include "EngineResource.h"
#include "SimpleMath.h"
#include "Graphics.h"
#include "guiWidget.h"

class GameObj;
namespace gui
{
	class WidgetMgr
	{
		SINGLE(WidgetMgr)

	public:

		void Initialize();
		void Release();
		void Run();

		void ImGui_Initialize();
		void ImGui_Run();
		void ImGui_Release();

		class Hierarchy* GetHierachy() { return mHierarchy; }
		class VisualEditor* GetVisualEditor() { return mVisualEditor; }
		
		template<typename T>
		void Insert(std::string name, T* widget)
		{
			auto iter = mWidgets.find(name);
			if (iter == mWidgets.end())
				mWidgets.insert(std::make_pair(name, widget));
		}

		template<typename T>
		T* GetWidget(const std::string& name)
		{
			auto iter = mWidgets.find(name);
			if (iter == mWidgets.end())
				return nullptr;

			return dynamic_cast<T*>(iter->second);
		}

	private:
		std::map<std::string, Widget*> mWidgets;

		class Hierarchy* mHierarchy;
		class VisualEditor* mVisualEditor;
	};

}

