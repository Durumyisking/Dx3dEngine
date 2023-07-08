#pragma once

namespace gui
{
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
		void DebugRender(dru::graphics::DebugMesh& mesh);

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
		std::map<std::string, class Widget*> mWidgets;
		std::vector<class EditorObject*> mEditorObjects;
		std::vector<class DebugObject*> mDebugObjects;

		class VisualEditor* mVisualEditor;
		bool mEnable;
		bool mImguiEnable;

	};
}
