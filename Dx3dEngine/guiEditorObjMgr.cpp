#include "guiEditorObjMgr.h"
#include "MeshRenderer.h"
#include "Renderer.h"
#include "ResourceMgr.h"
#include "InputMgr.h"

namespace gui
{
	EditorObjMgr::EditorObjMgr()
		: mDebugObjects{}
		, mEditorObjects{}
	{

	}

	EditorObjMgr::~EditorObjMgr()
	{
	}

	void EditorObjMgr::Initialize()
	{
		// �浹ü�� ���� ������ŭ�� ������ �ȴ�.
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
	}

	void EditorObjMgr::Release()
	{
		for (auto obj : mEditorObjects)
		{
			delete obj;
			obj = nullptr;
		}

		delete mDebugObjects[static_cast<UINT>(eColliderType::Rect)];
		mDebugObjects[static_cast<UINT>(eColliderType::Rect)] = nullptr;

		delete mDebugObjects[static_cast<UINT>(eColliderType::Circle)];
		mDebugObjects[static_cast<UINT>(eColliderType::Circle)] = nullptr;

		delete mDebugObjects[static_cast<UINT>(eColliderType::Box)];
		mDebugObjects[static_cast<UINT>(eColliderType::Box)] = nullptr;

		delete mDebugObjects[static_cast<UINT>(eColliderType::Sphere)];
		mDebugObjects[static_cast<UINT>(eColliderType::Sphere)] = nullptr;
	}

	void EditorObjMgr::Run()
	{
		Update();
		FixedUpdate();
		Render();
	}

	void EditorObjMgr::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->update();
		}
	}
	void EditorObjMgr::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->FixedUpdate();
		}
	}

	void EditorObjMgr::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->render();
		}

		for (DebugMesh& mesh : renderer::debugMeshes)
		{
			if (KEY_DOWN(V))
			{
				DebugRender(mesh);
			}
		}
		renderer::debugMeshes.clear();
	}

	void EditorObjMgr::DebugRender(DebugMesh& mesh)
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