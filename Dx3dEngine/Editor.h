#pragma once
#include "DebugObject.h"
#include "EditorObject.h"
#include "Widget.h"
#include "Graphics.h"

namespace dru
{
	class Editor
	{
	public:
		void Initialize();
		void Run();

		void update();
		void fixedUpdate();
		void render();
		void destroy();
		void debugRender(graphics::DebugMesh& mesh);

		std::shared_ptr<Mesh> GetLineMesh() const { mDebugObjects[(UINT)eColliderType::Line]->GetComponent<Mesh>(); }

	private:
		std::vector<Widget*> mWidgets;
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;
	};

}