#include "Editor.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "Input.h"

namespace dru
{
	void Editor::Initialize()
	{
		mDebugObjects.resize(static_cast<UINT>(eColliderType::End));

		// rectmesh
		std::shared_ptr<Mesh> rectMesh =  Resources::Find<Mesh>(L"DebugRectmesh");
		std::shared_ptr<Material> material = Resources::Find<Material>(L"DebugMaterial");

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		MeshRenderer* RectMeshrenderer = mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

		RectMeshrenderer->SetMaterial(material);
		RectMeshrenderer->SetMesh(rectMesh);

		// circlemesh
		std::shared_ptr<Mesh> CircleMesh = Resources::Find<Mesh>(L"Circlemesh");
		
		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		MeshRenderer* CicleMeshrenderer = mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

		CicleMeshrenderer->SetMaterial(material);
		CicleMeshrenderer->SetMesh(CircleMesh);

		// lineemesh
		std::shared_ptr<Mesh> LineMesh = Resources::Find<Mesh>(L"Linemesh");

		mDebugObjects[(UINT)eColliderType::Line] = new DebugObject();
		MeshRenderer* LineMeshrenderer = mDebugObjects[(UINT)eColliderType::Line]->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

		LineMeshrenderer->SetMaterial(material);
		LineMeshrenderer->SetMesh(LineMesh);


	}

	void Editor::Run()
	{
		update();
		fixedUpdate();
		render();
	}
	void Editor::update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->update();
		}
	}

	void Editor::fixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->fixedUpdate();
		}
	}

	void Editor::render()
	{
		if (Input::GetKeyDown(eKeyCode::V))
		{
			for (EditorObject* obj : mEditorObjects)
			{
				obj->render();
			}

			for (DebugMesh& mesh : renderer::debugMeshes)
			{
				debugRender(mesh);
			}
		}
		renderer::debugMeshes.clear();
	}

	void Editor::destroy()
	{
		for (auto* obj : mWidgets)
		{
			delete obj;
			obj = nullptr;
		}
		for (auto* obj : mEditorObjects)
		{
			delete obj;
			obj = nullptr;
		}

		delete mDebugObjects[(UINT)eColliderType::Rect];
		delete mDebugObjects[(UINT)eColliderType::Circle];
		delete mDebugObjects[(UINT)eColliderType::Line];
	}

	void Editor::debugRender(graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];
		
		Transform* tr = debugObj->GetComponent<Transform>();
//		tr->SetPosition(mesh.position);
		if (mainCamera->GetProjectionType() == eProjectionType::Perspective)
		{
			tr->SetPosition(mesh.position);
		}
		else
		{
			tr->SetPosition(Vector3(mesh.position.x, mesh.position.y, 0.1f));
		}

		tr->SetRotation(mesh.rotation);

		switch (mesh.type)
		{
		case dru::enums::eColliderType::Rect:
			tr->SetScale(mesh.scale);
			break;
		case dru::enums::eColliderType::Circle:
			tr->SetScale(Vector3(mesh.radius));
			break;
		case dru::enums::eColliderType::Line:
			tr->SetScale(Vector3(mesh.scale.x, 1.f, 0.f));
			break;
		case dru::enums::eColliderType::Box:
			break;
		case dru::enums::eColliderType::Sphere:
			break;
		case dru::enums::eColliderType::End:
			break;
		default:
			break;
		}

		
		BaseRenderer* renderer = debugObj->GetComponent<BaseRenderer>();

		std::shared_ptr<Material> material = renderer->GetMaterial();
		material->SetData(eGPUParam::Int_1, &mesh.state);
		tr->fixedUpdate(); // 행렬 다시만들어줌

		Camera::SetGpuViewMatrix(renderer::mainCamera->GetViewMatrix());
		Camera::SetGpuProjectionMatrix(renderer::mainCamera->GetProjectionMatrix());

		debugObj->render();
	}
}