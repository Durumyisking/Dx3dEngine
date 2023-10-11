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
#include "PhysXRayCast.h"

extern Application application;

namespace gui
{
	Editor::Editor()
		: mEnable(true)
	{
	}
	Editor::~Editor()
	{
	}

	void Editor::Initialize()
	{
		//mEnable = false;

		if (mEnable == false)
			return;

		GETSINGLE(EditorObjMgr)->Initialize();

		GETSINGLE(WidgetMgr)->Initialize();
	}

	void Editor::Run()
	{
		if (KEY_TAP(F_4))
		{
			if (mEnable == false)
			{
				mEnable = true;
				Initialize();
			}
			else
			{
				Release();
				mEnable = false;
			}
		}

		if (mEnable == false)
			return;

		GETSINGLE(EditorObjMgr)->Run();

		GETSINGLE(WidgetMgr)->Run();
	}

	void Editor::Release()
	{
		if (mEnable == false)
			return;

		GETSINGLE(WidgetMgr)->Release();

		GETSINGLE(EditorObjMgr)->Release();
	}

	void Editor::DestroySingle()
	{
		GETSINGLE(WidgetMgr)->DestroyInstance();
		GETSINGLE(EditorObjMgr)->DestroyInstance();
	}
}
