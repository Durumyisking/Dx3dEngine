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
		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize(static_cast<UINT>(eColliderType::End));

		Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"DebugMaterial");

		Mesh* rectMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Rectmesh");
		mDebugObjects[static_cast<UINT>(eColliderType::Rect)] = new DebugObject();
		MeshRenderer* renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Rect)]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		renderer->SetMaterial(material);
		renderer->SetMesh(rectMesh);

		mDebugObjects[static_cast<UINT>(eColliderType::Line)] = nullptr;

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

		Mesh* capsuleMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Rectmesh");
		mDebugObjects[static_cast<UINT>(eColliderType::Capsule)] = new DebugObject();
		renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Capsule)]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		renderer->SetMaterial(material);
		renderer->SetMesh(capsuleMesh);
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

		delete mDebugObjects[static_cast<UINT>(eColliderType::Capsule)];
		mDebugObjects[static_cast<UINT>(eColliderType::Capsule)] = nullptr;
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

		//for (DebugObject* obj: mDebugObjects)
		//{
		//	Transform* tr = obj->GetComponent<Transform>();

		//	if (nullptr == tr)
		//		return;

		//	Vector3 scale = tr->GetScale();
		//	//scale *= Vector3(mScale.x, mScale.y, 1.f);
		//	scale *= Vector3(1.f, 1.f, 1.f);

		//	Vector3 rotation = tr->GetRotation();

		//	Vector3 position = tr->GetWorldPosition();

		//	DebugMesh meshAttribute = {};
		//	meshAttribute.position = Vector3(position.x, position.y, position.z);
		//	meshAttribute.radius = 10.f;
		//	meshAttribute.rotation = rotation;
		//	meshAttribute.scale = scale;
		//	meshAttribute.type = enums::eColliderType::Box;
		//	meshAttribute.state = enums::eCollisionState::CollisionNot;


		//	renderer::debugMeshes.push_back(meshAttribute);
		//}
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
				//DebugRender(mesh);
			}
		}
		renderer::debugMeshes.clear();
	}

	void EditorObjMgr::DebugRender(DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[static_cast<UINT>(mesh.type)];

		if (debugObj == nullptr)
			return;

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
		case enums::eColliderType::Capsule:
			tr->SetScale(mesh.scale);
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