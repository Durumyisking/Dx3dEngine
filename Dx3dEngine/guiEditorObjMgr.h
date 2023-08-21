#pragma once
#include "Engine.h"
#include "EngineResource.h"
#include "SimpleMath.h"
#include "Graphics.h"
#include "guiEditorObject.h"
#include "guiDebugObject.h"

namespace gui
{
	class EditorObjMgr
	{
		SINGLE(EditorObjMgr)

	public:
		void Initialize();
		void Release();

		void Run();

		void Update();
		void FixedUpdate();
		void Render();
		void DebugRender(DebugMesh& mesh);
		
	private:
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;
	};
}

