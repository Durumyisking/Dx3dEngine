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
		//mLifePanal->InActive();
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
		directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 1000.f, 0.f));
		directionalLight->SetRotation(Vector3(90.f, 0.f, 0.f));
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
			L"mat_dirt"
		);
		player->GetComponent<MeshRenderer>()->SetMaterial(mat);

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
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
			L"mat_dirt"
		);
		player->GetComponent<MeshRenderer>()->SetMaterial(mat);

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}


	{
		//CubeMapHDR* sb = object::Instantiate<CubeMapHDR>(eLayerType::CubeMap);
		//Texture* t = GETSINGLE(ResourceMgr)->Find<Texture>(L"night11");
		//sb->SetTexture(t);
	}
	{
		SkySphere* player = object::Instantiate<SkySphere>(eLayerType::SkySphere);
		player->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		player->GetComponent<Transform>()->SetScale(Vector3(500.0f, 500.0f, 500.0f));
		player->SetName(L"SkySphere");
	}
	
	{
		GameObj* plane = object::Instantiate<GameObj>(eLayerType::Platforms);
		plane->SetPos(Vector3(0.f, -0.251f, 0.f));
		plane->SetScale({ 1000.f, 0.5f, 1000.f });
		plane->SetName(L"Plane");
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"PBRMaterial");
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
			
	//LifePanal
	{
		mLifePanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(6.4f, 3.1f, 10.f), Vector3(1.0f, 1.0f, 1.0f), L"LifePanal"));


		//LifeGauge
		HUD* hud = object::Instantiate<HUD>(eLayerType::UI);
		hud->SetPos(Vector3(0.f, 0.f, 0.f));
		hud->SetScale({ 1.f, 1.f, 1.f });
		hud->SetName(L"hud2");
		MeshRenderer* hudRender = hud->AddComponent<MeshRenderer>(eComponentType::UI);
		hudRender->SetMeshByKey(L"Rectmesh");
		hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"LifeGauge_3Material"));
		hud->GetComponent<Transform>()->SetParent(mLifePanal);


		//Lifeheart
		HUD* lifeheart = object::Instantiate<HUD>(eLayerType::UI);
		lifeheart->SetPos(Vector3(0.f, -0.03f, 0.f));
		lifeheart->SetScale({ 0.6f, 0.6f, 1.f });
		lifeheart->SetName(L"Lifeheart");
		MeshRenderer* lifeheartRender = lifeheart->AddComponent<MeshRenderer>(eComponentType::UI);
		lifeheartRender->SetMeshByKey(L"Rectmesh");
		lifeheartRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"LifeheartMaterial"));
		lifeheart->GetComponent<Transform>()->SetParent(mLifePanal);
	}

	//Left Coin UI
	{
		mCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(-6.4f, 3.1f, 10.f), Vector3(0.7f,0.7f,1.0f), L"CoinPanal"));


		HUD* coin = object::Instantiate<HUD>(eLayerType::UI);
		coin->SetPos(Vector3(0.f, 0.f, 0.f));
		coin->SetScale({ 1.f, 1.f, 1.f });
		coin->SetName(L"Coin");
		MeshRenderer* coinRender = coin->AddComponent<MeshRenderer>(eComponentType::UI);
		coinRender->SetMeshByKey(L"Rectmesh");
		coinRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"CoinMaterial"));
		coin->GetComponent<Transform>()->SetParent(mCoinPanal);

		HUD* cityCoin = object::Instantiate<HUD>(eLayerType::UI);
		cityCoin->SetPos(Vector3(3.f, 0.1f, 0.f));
		cityCoin->SetScale({ 1.f, 1.f, 1.f });
		cityCoin->SetName(L"CityCoin");
		MeshRenderer* cityCoinRender = cityCoin->AddComponent<MeshRenderer>(eComponentType::UI);
		cityCoinRender->SetMeshByKey(L"Rectmesh");
		cityCoinRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"CityCoinMaterial"));
		cityCoin->GetComponent<Transform>()->SetParent(mCoinPanal);

		HUD* bar = object::Instantiate<HUD>(eLayerType::UI);
		bar->SetPos(Vector3(2.f, -0.55f, 0.f));
		bar->SetScale({ 5.f, 1.4f, 1.f });
		bar->SetRotation({ 0.f, 0.f, 1.f });
		bar->SetName(L"Bar");
		MeshRenderer* barRender = bar->AddComponent<MeshRenderer>(eComponentType::UI);
		barRender->SetMeshByKey(L"Rectmesh");
		barRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"BarMaterial"));
		bar->GetComponent<Transform>()->SetParent(mCoinPanal);
	}

	//left Luna UI
	{
		mLunaPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(-6.4f, 2.0f, 10.f),Vector3::One ,L"LunaPanal"));


		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				HUD* luna = object::Instantiate<HUD>(eLayerType::UI);
				luna->SetPos(Vector3(-0.11f + (0.33f * i), (0.40f - (0.35f * j)) + (0.01f * i), 0.f));
				luna->SetScale({ 0.3f, 0.3f, 1.f });
				luna->SetName(L"Luna" + i + (j * 10));

				MeshRenderer* lunaRender = luna->AddComponent<MeshRenderer>(eComponentType::UI);
				lunaRender->SetMeshByKey(L"Rectmesh");
				lunaRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"LunaMaterial"));
				luna->GetComponent<Transform>()->SetParent(mLunaPanal);
				luna->Die();
			}
		}

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				HUD* dottedLine = object::Instantiate<HUD>(eLayerType::UI);
				dottedLine->SetPos(Vector3(-0.11f + (0.33f * i), (0.40f - (0.35f * j)) + (0.01f * i), 0.f));
				dottedLine->SetScale({ 0.3f, 0.3f, 1.f });
				dottedLine->SetName(L"DottedLine");

				MeshRenderer* dottedLineRender = dottedLine->AddComponent<MeshRenderer>(eComponentType::UI);
				dottedLineRender->SetMeshByKey(L"Rectmesh");
				dottedLineRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"DottedLineMaterial"));
				dottedLine->GetComponent<Transform>()->SetParent(mLunaPanal);
			}
		}
	}

	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}