#include "guiEditor.h"
#include "Mesh.h"
#include "ResourceMgr.h"
#include "Material.h"
#include "guiMaterial.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "GridScript.h"
#include "Object.h"
#include "Application.h"
#include "GraphicDevice.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "guiInspector.h"
#include "guiGame.h"
#include "guiHierarchy.h"
#include "guiProject.h"
#include "guiEditor.h"
#include "guiConsole.h"
#include "guiListWidget.h"

#include "guiVisualEditor.h"

#include "guiDebugObject.h"
#include "guiEditorObject.h"
#include "guiWidget.h"
#include "Graphics.h"

#include "ResourceMgr.h"
#include "InputMgr.h"

#include "guiWidgetMgr.h"
#include "guiEditorObjMgr.h"

extern Application application;

namespace gui
{
	Editor::Editor()
		: mEnable(false)
		, mImguiEnable(true)

	{
	}
	Editor::~Editor()
	{
	}

	void Editor::Initialize()
	{
		mEnable = true;
		mImguiEnable = true;

		if (mEnable == false)
			return;

		GETSINGLE(EditorObjMgr)->Initialize();

		if (mImguiEnable == false)
			return;

		GETSINGLE(WidgetMgr)->Initialize();
	}

	void Editor::Run()
	{
		if (mEnable == false)
			return;

		GETSINGLE(EditorObjMgr)->Run();

		if (mImguiEnable == false)
			return;

		GETSINGLE(WidgetMgr)->Render();
	}

	void Editor::Release()
	{
		if (mEnable == false)
			return;

		GETSINGLE(EditorObjMgr)->Release();

		if (mImguiEnable == false)
			return;
		
		GETSINGLE(WidgetMgr)->Release();
	}

	void Editor::DestroySingle()
	{
		GETSINGLE(WidgetMgr)->DestroyInstance();
		GETSINGLE(EditorObjMgr)->DestroyInstance();
	}
}
