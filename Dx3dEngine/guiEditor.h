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
		void DestroySingle();

		void DebugRender(DebugMesh& mesh);

	private:
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;

		bool mEnable;
		bool mImguiEnable;
	};
}
