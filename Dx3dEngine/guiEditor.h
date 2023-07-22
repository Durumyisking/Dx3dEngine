#pragma once
#include "Engine.h"
#include "Graphics.h"

namespace gui
{
	class Widget;
	class EditorObject;
	class DebugObject;
	class Editor
	{
	public:
		Editor();
		virtual ~Editor();

		void Initialize();
		void Run();

		void Update();
		void FixedUpdate();
		void Render();
		void Release();
		void DebugRender(DebugMesh& mesh);

		void ImGui_Initialize();
		void ImGui_Run();
		void ImGui_Release();

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
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;

		class VisualEditor* mVisualEditor;
		bool mEnable;
		bool mImguiEnable;

	};
}
