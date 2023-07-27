#include "SceneTitle.h"
#include "TimeMgr.h"

#include "Object.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Texture.h"
#include "Camera.h"
#include "CameraScript.h"
#include "FontWrapper.h"

#include "GridScript.h"

#include "Application.h"
#include "Player.h"

#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PlayerScript.h"
#include "PhysicalMovement.h"
#include "PhysicsMgr.h"
#include "PhysicsScene.h"
#include "PhysX.h"

#include "Sphere.h"
#include "Box.h"

#include "ServerMgr.h"

#include "InputMgr.h"

extern Application application;


SceneTitle::SceneTitle()
	: mCamera(nullptr)

{
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Initialize()
{
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Platforms, eLayerType::Player);

	Scene::Initialize();
}

void SceneTitle::update()
{
	if (KEY_TAP(N_9))
	{
		GETSINGLE(server::ServerMgr)->OpenServer();
	}
	if (KEY_TAP(N_0))
	{
		GETSINGLE(server::ServerMgr)->ConnectAsClient();
	}


	Scene::update();
}

void SceneTitle::fixedUpdate()
{
	Scene::fixedUpdate();
}

void SceneTitle::render()
{

}

void SceneTitle::Enter()
{
	//mDeleteObj = true;

	{
		mCamera = object::Instantiate<GameObj>(eLayerType::Camera, L"MainCam");
		Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraComp->SmoothOn();
		mCamera->AddComponent<CameraScript>(eComponentType::Script);
		renderer::mainCamera = cameraComp;
		cameraComp->SetProjectionType(eProjectionType::Perspective);
		mCamera->SetPos(Vector3(0.f, 15.f, 0.f));

	}


	{
		GameObj* gridObject = object::Instantiate<GameObj>(eLayerType::Grid, L"Grid");

		MeshRenderer* gridMr = gridObject->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

		gridMr->SetMesh(GETSINGLE(ResourceMgr)->Find<Mesh>(L"Gridmesh"));
		gridMr->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"GridMaterial"));
		gridMr->LODOff();

		GridScript* gridScript = gridObject->AddComponent<GridScript>(eComponentType::Script);
		gridScript->SetCamera(mainCamera);

		float w = static_cast<float>(application.GetWidth());
		float h = static_cast<float>(application.GetHeight());
		gridObject->SetPos({ 0.f, 0.f, 0.f });
		gridObject->SetScale(Vector3(1.f, 1.f, 1.f));
	}

	{
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLightTitleScene");
		directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 500.f, -1000.f));
		directionalLight->SetRotation(Vector3(45.f, 0.f, 0.f));
		directionalLight->SetScale(Vector3(15.f, 15.f, 15.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetAmbient(Vector4(0.5f, 0.5f, 0.5f, 1.f));
		MeshRenderer* mr = directionalLight->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		mr->SetMaterialByKey(L"SunMaterial");
		mr->ChangeColor(Vector4(1.f, 1.f, 1.f, 1.f));
	}


	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(5.f, 5.f, 5.f));
		player->SetScale({ 5.f, 5.f, 5.f });
		player->SetName(L"Player");
		//Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"dirt_color", L"dirt_normal", L"PhongShader", L"mat_dirt");
		//Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"BrickBlockBody_alb", L"BrickBlockBody_nrm", L"PhongShader", L"mat_BrickBlockBody");
		//Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"BrickBlockBody_alb", L"PhongShader", L"mat_BrickBlockBody");
		//player->GetComponent<MeshRenderer>()->SetMaterial(mat);
		player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PBRMaterial");
		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(2.5f, 2.5f, 2.5f));
		PxRigidDynamic* dy = physical->GetActor<PxRigidDynamic>();

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	//{
	//	
	//	Sphere* sphere = object::Instantiate<Sphere>(eLayerType::PhysicalObject);
	//	sphere->SetPos(Vector3(-5.f, 20.f, 5.f));
	//	sphere->SetScale({ 2.5f, 2.5f, 2.5f });
	//	sphere->SetName(L"Sphere");
	//	sphere->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");

	//}


	{
		GameObj* plane = object::Instantiate<GameObj>(eLayerType::Platforms);
		plane->SetPos(Vector3(0.f, -0.251f, 0.f));
		plane->SetScale({ 1000.f, 0.5f, 1000.f });
		plane->SetName(L"Plane");
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"PhongMaterial");
		plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		plane->AddComponent<PhysXCollider>(eComponentType::Collider);
	}


	//Test
	{
		{
			GameObj* test1 = object::Instantiate<GameObj>(eLayerType::Objects);
			test1->SetPos(Vector3(0.f, 5.f, 0.f));
			test1->SetScale({ 5.f, 5.f, 5.f });
			test1->SetName(L"Test1");
			Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"Brick_Color", L"Brick_Normal", L"PhongShader", L"Brick_Phong_Mat");
			test1->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
			test1->GetComponent<MeshRenderer>()->SetMaterial(mat);
			test1->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
			//player->AddComponent<PlayerScript>(eComponentType::Script);

			//Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
			//physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(2.5f, 2.5f, 2.5f));
			//PxRigidDynamic* dy = physical->GetActor<PxRigidDynamic>();

			//PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

			//player->AddComponent<PhysXCollider>(eComponentType::Collider);
			//player->AddComponent<PhysicalMovement>(eComponentType::Movement);
		}

		// DebugTest
		{
			GameObj* test2 = object::Instantiate<GameObj>(eLayerType::Objects);
			test2->SetPos(Vector3(10.f, 5.f, 0.f));
			test2->SetScale({ 5.f, 5.f, 5.f });
			test2->SetName(L"Test2");

			test2->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
			MeshRenderer* testRender = test2->GetComponent<MeshRenderer>();
			Material* testMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"DeferredMaterial");

			testRender->SetMaterial(testMaterial);
			testRender->SetMeshByKey(L"Cubemesh");
		}

		{
			GameObj* test3 = object::Instantiate<GameObj>(eLayerType::Objects);
			test3->SetPos(Vector3(-10.f, 5.f, 0.f));
			test3->SetScale({ 5.f, 5.f, 5.f });
			test3->SetName(L"Test3");
			Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"Brick_Color", L"Brick_Normal", L"BasicShader", L"Basic_Test_Mat");
			test3->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
			test3->GetComponent<MeshRenderer>()->SetMaterial(mat);
			test3->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
		}
	}

	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}
