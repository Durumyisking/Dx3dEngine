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

extern Application application;

namespace gui
{
	Editor::Editor()
		: mEditorObjects{}
		, mDebugObjects{}
		, mEnable(false)
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
		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize(static_cast<UINT>(eColliderType::End));

		Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"DebugMaterial");

		Mesh* rectMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Rectmesh");
		mDebugObjects[static_cast<UINT>(eColliderType::Rect)] = new DebugObject();
		MeshRenderer* renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Rect)]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		renderer->SetMaterial(material);
		renderer->SetMesh(rectMesh);


		Mesh* circleMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Circlemesh");
		mDebugObjects[static_cast<UINT>(eColliderType::Circle)] = new DebugObject();
		renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Circle)]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		renderer->SetMaterial(material);
		renderer->SetMesh(circleMesh);


		Mesh* cubeMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");
		mDebugObjects[static_cast<UINT>(eColliderType::Box)] = new DebugObject();
		renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Box)]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		renderer->SetMaterial(material);
		renderer->SetMesh(cubeMesh);


		Mesh* sphereMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Spheremesh");
		mDebugObjects[static_cast<UINT>(eColliderType::Sphere)] = new DebugObject();
		renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Sphere)]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		renderer->SetMaterial(material);
		renderer->SetMesh(sphereMesh);

		if (mImguiEnable == false)
			return;

		GETSINGLE(WidgetMgr)->Initialize();
	}

	void Editor::Run()
	{
		if (mEnable == false)
			return;

		Update();
		FixedUpdate();
		Render();

		if (mImguiEnable == false)
			return;

		GETSINGLE(WidgetMgr)->Render();
	}

	void Editor::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->update();
		}
	}

	void Editor::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->fixedUpdate();
		}
	}

	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->render();
		}

		for (DebugMesh & mesh : renderer::debugMeshes)
		{
			if (KEY_DOWN(V))
			{
				DebugRender(mesh);
			}
		}
		renderer::debugMeshes.clear();
	}

	void Editor::Release()
	{
		if (mEnable == false)
			return;

		for (auto obj : mEditorObjects)
		{
			delete obj;
			obj = nullptr;
		}

		delete mDebugObjects[static_cast<UINT>(eColliderType::Rect)];
		delete mDebugObjects[static_cast<UINT>(eColliderType::Circle)];
		delete mDebugObjects[static_cast<UINT>(eColliderType::Box)];
		delete mDebugObjects[static_cast<UINT>(eColliderType::Sphere)];

		if (mImguiEnable == false)
			return;
		
		GETSINGLE(WidgetMgr)->Release();
	}

	void Editor::DestroySingle()
	{
		GETSINGLE(WidgetMgr)->DestroyInstance();
	}

	void Editor::DebugRender(DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[static_cast<UINT>(mesh.type)];

		Transform* tr = debugObj->GetComponent<Transform>();
		tr->SetPosition(mesh.position);
		tr->SetRotation(mesh.rotation);

		switch (mesh.type)
		{
		case enums::eColliderType::Rect:
			tr->SetScale(mesh.scale);
			break;
		case enums::eColliderType::Circle:
			tr->SetScale(Vector3(mesh.radius));
			break;
		case enums::eColliderType::Line:
			tr->SetScale(Vector3(mesh.scale.x, 1.f, 0.f));
			break;
		case enums::eColliderType::Box:
			tr->SetScale(mesh.scale);
			break;
		case enums::eColliderType::Sphere:
			tr->SetScale(Vector3(mesh.scale.x));
			break;
		case enums::eColliderType::End:
			break;
		default:
			break;
		}

		BaseRenderer* renderer = debugObj->GetComponent<BaseRenderer>();

		Material* material = renderer->GetMaterial();
		material->SetData(eGPUParam::Int_1, &mesh.state);
		tr->FixedUpdate();

		Camera::SetGpuViewMatrix(renderer::mainCamera->GetViewMatrix());
		Camera::SetGpuProjectionMatrix(renderer::mainCamera->GetProjectionMatrix());

		debugObj->Render();

	}

}
