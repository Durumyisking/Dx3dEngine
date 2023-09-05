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

#include "CubeMap.h"
#include "SkySphere.h"

#include "BoneAnimator.h"

#include "Panal.h"
#include "HUD.h"
#include "Button.h"
#include "UIFactory.h"
#include "Animator.h"


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


	if (KEY_TAP(N_1))
	{
		MainMenuPanal->InActivate();

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
		// UI Camera
		mUICamera = object::Instantiate<GameObj>(eLayerType::Camera);
		Camera* cameraUIComp = mUICamera->AddComponent<Camera>(eComponentType::Camera);
		mUICamera->AddComponent<CameraScript>(eComponentType::Script);

		cameraUIComp->SetProjectionType(eProjectionType::Perspective);
		cameraUIComp->SmoothOn();
		cameraUIComp->DisableLayerMasks();
		cameraUIComp->TurnLayerMask(eLayerType::UI, true);
		mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	}

	
	//{
	//	GameObj* gridObject = object::Instantiate<GameObj>(eLayerType::Grid, L"Grid");

	//	MeshRenderer* gridMr = gridObject->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	//	gridMr->SetMesh(GETSINGLE(ResourceMgr)->Find<Mesh>(L"Gridmesh"));
	//	gridMr->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"GridMaterial"));
	//	gridMr->LODOff();

	//	GridScript* gridScript = gridObject->AddComponent<GridScript>(eComponentType::Script);
	//	gridScript->SetCamera(mainCamera);

	//	float w = static_cast<float>(application.GetWidth());
	//	float h = static_cast<float>(application.GetHeight());
	//	gridObject->SetPos({ 0.f, 0.f, 0.f });
	//	gridObject->SetScale(Vector3(1.f, 1.f, 1.f));
	//}

	{
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLightTitleScene");
		directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 1000.f, 0.f));
		directionalLight->SetRotation(Vector3(45.f, 0.f, 0.f));
		directionalLight->SetScale(Vector3(15.f, 15.f, 15.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
	}
	

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(5.f, 5.f, 5.f));
		player->SetScale(Vector3(1.f, 1.f, 1.f));
		player->SetName(L"Player");
		Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial
		(
			L"gold_albedo",
			L"gold_normal", 
			L"gold_metallic", 
			L"gold_roughness", 
			L"PBRShader",
			L"gold_dirt"
		);
		player->GetComponent<MeshRenderer>()->SetMaterial(mat);

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-5.f, 5.f, 5.f));
		player->SetScale(Vector3(1.f, 1.f, 1.f));
		player->SetName(L"Player");
		Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial
		(
			L"check_albedo",
			L"check_normal",
			L"check_metallic",
			L"check_roughness",
			L"PBRShader",
			L"check_dirt"
		);
		player->GetComponent<MeshRenderer>()->SetMaterial(mat);

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 5.f, 5.f));
		player->SetScale(Vector3(1.f, 1.f, 1.f));
		player->SetName(L"Player");
		Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial
		(
			L"iron_albedo",
			L"iron_normal",
			L"iron_metallic",
			L"iron_roughness",
			L"PBRShader",
			L"wood_dirt"
		);
		player->GetComponent<MeshRenderer>()->SetMaterial(mat);

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}


	{
		CubeMapHDR* cubeMap = object::Instantiate<CubeMapHDR>(eLayerType::CubeMap);

		Texture* t = GETSINGLE(ResourceMgr)->Find<Texture>(L"night11");
		t->BindAllShaderResource(12);
	}
	{
		SkySphere* skySphere = object::Instantiate<SkySphere>(eLayerType::SkySphere);
		skySphere->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		skySphere->SetName(L"SkySphere");
	}
	
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

	{ 
		//Deferred
		//{
		//	GameObj* test2 = object::Instantiate<GameObj>(eLayerType::Objects);
		//	test2->SetPos(Vector3(-10.f, 5.f, 0.f));
		//	test2->SetScale({ 5.f, 5.f, 5.f });
		//	test2->SetName(L"Test2");

		//	test2->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		//	MeshRenderer* testRender = test2->GetComponent<MeshRenderer>();
		//	Material* testMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"DeferredMaterial");

		//	testRender->SetMaterial(testMaterial);
		//	testRender->SetMeshByKey(L"Cubemesh");
		//}		
	}
	//   GameObj* test1 = object::Instantiate<GameObj>(eLayerType::Objects);
	   //test1->SetPos(Vector3(-5.f, 5.f, 0.f));
	   //test1->SetScale({ 5.f, 5.f, 5.f });

	   //test1->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	   //MeshRenderer* testRender = test1->GetComponent<MeshRenderer>();

	   ////SpriteRenderer* spriteRender = test1->AddComponent<SpriteRenderer>(eComponentType::UI);
	   ////Texture* titleTexture = (GETSINGLE(ResourceMgr)->Find<Texture>(L"MarioTitle"));

	   //Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"RectMesh");
	   //testRender->SetMesh(mesh);
	   //Material* mat = GETSINGLE(ResourceMgr)->Find<Material>(L"UIMaterial");
	   ////mat->SetTexture(titleTexture);
	   //testRender->SetMaterial(mat);
	   //testRender->SetMeshByKey(L"Cubemesh");
	   // 
	//UISpriteMaterial
	{

		//HUD* hud = object::Instantiate<HUD>(eLayerType::UI);
		//hud->SetPos(Vector3(-5.f, 2.f, 10.f));
		//hud->SetScale({ 1.f, 1.f, 1.f });
		//hud->SetName(L"hud");
		//
		//MeshRenderer* hudRender = hud->AddComponent<MeshRenderer>(eComponentType::UI);
		//hudRender->SetMeshByKey(L"Rectmesh");
		//hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"UISpriteMaterial"));

		//hud->GetComponent<Transform>()->SetParent(mUICamera);


	}
	CreateMainMenu();
	CreatePlayerUI();


	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::CreateMainMenu()
{
	MainMenuPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 1.f), Vector3(1.0f, 1.0f, 1.0f), L"WorldMapPanal"));

	HUD* worldMap = (GETSINGLE(UIFactory)->CreateHud(L"WorldMap", L"WorldMapMaterial", Vector3(0.f, 0.6f, 0.f), Vector3(3.f, 3.f, 1.f), MainMenuPanal));
	HUD* filter = (GETSINGLE(UIFactory)->CreateHud(L"RedFilter", L"FilterMaterial", Vector3(0.f, 0.6f, 0.f), Vector3(3.f, 3.f, 1.f), MainMenuPanal));
	worldMap->SetState(HUDState::Rotate);
	worldMap->SetSpeed(1);
	HUD* title = (GETSINGLE(UIFactory)->CreateHud(L"MarioTitle", L"TitleMaterial", Vector3(-0.6f, 0.3f, 0.f), Vector3(0.2f, 0.2f, 1.f), MainMenuPanal));
	HUD* bar = (GETSINGLE(UIFactory)->CreateHud(L"UIBar", L"UIBarMaterial", Vector3(-0.4f, 0.f, 0.f), Vector3(0.6f, 0.08f, 1.f), MainMenuPanal));
	bar->SetRotation(Vector3(0.0f, 0.0f, 2.0f));
	bar->SetState(HUDState::MoveBlink);
	HUD* cap = (GETSINGLE(UIFactory)->CreateHud(L"Cap", L"CapMaterial", Vector3(-0.35f, 0.0f, 0.f), Vector3(0.15f, 0.45f, 1.f), bar));
	//Animator* capAni = cap->AddComponent<Animator>(eComponentType::Animator);
	//Texture* tex = (GETSINGLE(ResourceMgr)->Find<Texture>(L"CapRotate"));
	//capAni->Create(L"CapAni", tex, Vector2::Zero, Vector2(84.0f, 50.0f), Vector2::One, 5, Vector2(100.0f, 80.0f), 0.1f);
	//capAni->Play(L"CapAni", true);
	MainMenuPanal->Addchild(worldMap);
	MainMenuPanal->Addchild(filter);
	MainMenuPanal->Addchild(title);
	MainMenuPanal->Addchild(bar);
	MainMenuPanal->Addchild(cap);
}

void SceneTitle::CreatePlayerUI()
{
	//Life UI
	{
		mLifePanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(6.4f, 3.1f, 10.f), Vector3(1.0f, 1.0f, 1.0f), L"LifePanal"));
		//LifeGauge
		HUD* hud = (GETSINGLE(UIFactory)->CreateHud(L"LifeGauge", L"LifeGauge_3Material", Vector3(0.f, 0.f, 0.f), Vector3::One, mLifePanal));
		//Lifeheart
		HUD* lifeheart = (GETSINGLE(UIFactory)->CreateHud(L"LifeHeart", L"LifeheartMaterial", Vector3(0.f, -0.03f, 0.f), Vector3(0.6f, 0.6f, 1.0f), mLifePanal));

		mLifePanal->Addchild(hud);
		mLifePanal->Addchild(lifeheart);
	}

	//Left Coin UI
	{
		mCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(-6.4f, 3.1f, 10.f), Vector3(0.7f, 0.7f, 1.0f), L"CoinPanal"));
		HUD* coin = (GETSINGLE(UIFactory)->CreateHud(L"Coin", L"CoinMaterial", Vector3(0.f, 0.f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal));
		HUD* cityCoin = (GETSINGLE(UIFactory)->CreateHud(L"CityCoin", L"CityCoinMaterial", Vector3(3.f, 0.1f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal));
		HUD* bar = (GETSINGLE(UIFactory)->CreateHud(L"Bar", L"BarMaterial", Vector3(2.f, -0.55f, 0.f), Vector3(5.f, 1.4f, 1.f), mCoinPanal));
		mCoinPanal->Addchild(coin);
		mCoinPanal->Addchild(cityCoin);
		mCoinPanal->Addchild(bar);
	}

	//left Luna UI
	{
		mLunaPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(-6.4f, 2.0f, 10.f), Vector3::One, L"LunaPanal"));

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				Vector3 position = Vector3(-0.11f + (0.33f * i), (0.40f - (0.35f * j)) + (0.01f * i), 0.f);


				HUD* luna = (GETSINGLE(UIFactory)->CreateHud(L"Luna" + std::to_wstring(i) + std::to_wstring(j * 10), L"LunaMaterial",
					position,
					Vector3(0.3f, 0.3f, 1.f),
					mLunaPanal));
				luna->Die();
				mLunaPanal->Addchild(luna);

				HUD* dottedLine = (GETSINGLE(UIFactory)->CreateHud(L"DottedLine" + std::to_wstring(i) + std::to_wstring(j * 10), L"DottedLineMaterial",
					position,
					Vector3(0.3f, 0.3f, 1.f),
					mLunaPanal));

				mLunaPanal->Addchild(dottedLine);
			}
		}
	}
}
