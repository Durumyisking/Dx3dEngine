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
#include "Model.h"
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

#include "SkyBox.h"

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
		mCamera->SetPos(Vector3(0.f, 5.f, -20.f));

	}
	
	{
		SkyBox* box = object::Instantiate<SkyBox>(eLayerType::BackGround, L"TitleSkyBox");
		box->TextureLoad(L"TitleSky", L"..//Resources//SkyCityNightStar_color.png");

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
//		lightComp->SetAmbient(Vector4(0.5f, 0.5f, 0.5f, 1.f));
		//MeshRenderer* mr = directionalLight->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		//mr->SetMaterialByKey(L"SunMaterial");
		//mr->ChangeColor(Vector4(1.f, 1.f, 1.f, 1.f));
	}
	

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(5.f, 5.f, 5.f));
		player->SetScale(Vector3(1.f, 1.f, 1.f));
		player->SetName(L"Player");
		Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial
		(
			L"check_albedo",
			L"check_normal", 
			L"check_metallic", 
			L"check_roughness", 
			L"PBRShader",
			L"mat_dirt"
		);
		player->GetComponent<MeshRenderer>()->SetMaterial(mat);

		//player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	
	{

	/*	GameObj* player = object::Instantiate<GameObj>(eLayerType::Objects);
		player->SetPos(Vector3(10.f, 0.f, 0.f));
		player->SetScale({ 0.01f, 0.01f, 0.01f });
		player->SetName(L"Object");

		MeshRenderer* meshRenderer = player->AddComponent<MeshRenderer>(eComponentType::Renderer);

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"blockBrick");
		meshRenderer->SetModel(model);
		meshRenderer->SetMaterialByKey(L"BlockBrickBody");

		player->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(0.5f, 1.f, 0.5f));
		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);*/
	}
	
	{
		/*GameObj* plane = object::Instantiate<GameObj>(eLayerType::Platforms);
		plane->SetPos(Vector3(0.f, -0.251f, 0.f));
		plane->SetScale({ 1000.f, 0.5f, 1000.f });
		plane->SetName(L"Plane");
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"PhongMaterial");
		plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		plane->AddComponent<PhysXCollider>(eComponentType::Collider);*/
	}


	//{
		//GameObj* skyBox = object::Instantiate<GameObj>(eLayerType::Objects);
		//skyBox->SetPos(Vector3(0.f, 5.f, 0.f));
		//skyBox->SetScale({ 5.f, 5.f, 5.f });
		//skyBox->SetName(L"Skybox");
		//Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial
		//(
		//	L"Skybox",
		//	eTextureSlot::Skybox,
		//	L"SkyboxShader",
		//	L"SkyboxMat"
		//);
		//MeshRenderer* mr = skyBox->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		//mr->SetMaterial(mat);
		//mr->SetMeshByKey(L"Spheremesh");
	//}


	// DebugTest
	{ 
		//Deferred
		{
			GameObj* test2 = object::Instantiate<GameObj>(eLayerType::Objects);
			test2->SetPos(Vector3(-10.f, 5.f, 0.f));
			test2->SetScale({ 5.f, 5.f, 5.f });
			test2->SetName(L"Test2");

			test2->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
			MeshRenderer* testRender = test2->GetComponent<MeshRenderer>();
			Material* testMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"DeferredMaterial");

			testRender->SetMaterial(testMaterial);
			testRender->SetMeshByKey(L"Cubemesh");
		}		
	}

	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}
