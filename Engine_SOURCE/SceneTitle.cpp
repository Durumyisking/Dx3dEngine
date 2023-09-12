#include "SceneTitle.h"
#include "TimeMgr.h"

#include "Object.h"
#include "Camera.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Texture.h"
#include "CameraScript.h"
#include "Model.h"
#include "FontWrapper.h"

#include "GridScript.h"

#include "Application.h"
#include "Player.h"
<<<<<<< HEAD
#include "MarioParts.h"
=======
#include "Goomba.h"
#include "Packun.h"
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d

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
<<<<<<< HEAD
#include "Monster.h"
=======
#include "HUD.h"
#include "Button.h"
#include "UIFactory.h"
#include "Animator.h"

>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d

extern Application application;


SceneTitle::SceneTitle()
	: mCamera(nullptr)
	, mUICamera(nullptr)
	, MainMenuPanal(nullptr)

{
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Initialize()
{
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Platforms, eLayerType::Player);


	//mDeleteObj = true;

	{
		mCamera = object::Instantiate<GameObj>(eLayerType::Camera,this, L"MainCamera");
		Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraComp->SmoothOn();
		//mCamera->AddComponent<CameraScript>(eComponentType::Script);
		renderer::mainCamera = cameraComp;
		cameraComp->SetProjectionType(eProjectionType::Perspective);
		mCamera->SetPos(Vector3(0.f, 5.f, -20.f));
	}
	  
	{
		// UI Camera
		mUICamera = object::Instantiate<GameObj>(eLayerType::Camera,this, L"UICamera");
		Camera* cameraUIComp = mUICamera->AddComponent<Camera>(eComponentType::Camera);
		mUICamera->AddComponent<CameraScript>(eComponentType::Script);

		cameraUIComp->SetProjectionType(eProjectionType::Perspective);
		cameraUIComp->SmoothOn();
		cameraUIComp->DisableLayerMasks();
		cameraUIComp->TurnLayerMask(eLayerType::UI, true);
		mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	}


	CreateMainMenu();


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


<<<<<<< HEAD
	/*for (auto& i : mroot)
	{
		i->getcomponent<transform>()->setposition(mrootgameobj->getcomponent<transform>()->getposition());
		i->getcomponent<transform>()->setrotation(mrootgameobj->getcomponent<transform>()->getrotation());
	}*/
=======
	if (KEY_TAP(N_1))
	{
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Play);
		return;
	}
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d

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
	renderer::mainCamera = mCamera->GetComponent<Camera>();
	mCamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mCamera->SetRotation(Vector3::Zero);
	mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mUICamera->SetRotation(Vector3::Zero);

	{
<<<<<<< HEAD
		mCamera = object::Instantiate<GameObj>(eLayerType::Camera, L"MainCam");
		Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraComp->SmoothOn();
		mCamera->AddComponent<CameraScript>(eComponentType::Script);
		renderer::mainCamera = cameraComp;
		cameraComp->SetProjectionType(eProjectionType::Perspective);
		mCamera->SetPos(Vector3(0.f, 5.f, -20.f));

	}

	//{
	//	// UI Camera
	//	mUICamera = object::Instantiate<GameObj>(eLayerType::Camera);
	//	Camera* cameraUIComp = mUICamera->AddComponent<Camera>(eComponentType::Camera);
	//	mUICamera->AddComponent<CameraScript>(eComponentType::Script);

	//	cameraUIComp->SetProjectionType(eProjectionType::Perspective);
	//	cameraUIComp->SmoothOn();
	//	cameraUIComp->DisableLayerMasks();
	//	cameraUIComp->TurnLayerMask(eLayerType::UI, true);
	//	mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	//}

	
	{
		/*SkyBox* box = object::Instantiate<SkyBox>(eLayerType::BackGround, L"TitleSkyBox");
		box->TextureLoad(L"TitleSky", L"..//Resources//SkyCityNightStar_color.png");*/


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
=======
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLight");
		directionalLight->SetRotation(Vector3(45.f, -45.f, 0.f));
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
		directionalLight->SetScale(Vector3(15.f, 15.f, 15.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
	}
<<<<<<< HEAD
	
	//플레이어 호출, 호출시 알아서 모델 initialize
	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
	}



	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-100.f, -100.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"BirdCity");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/BirdCity/Animation/FlyLanding.smd", 0.05f);
		animator->Play(L"test");*/
	}

	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CapMan");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/CapMan/Animation/Give.smd", 0.05f);
		animator->Play(L"test");*/
	}

	{
	/*	Monster* player = object::Instantiate<Monster>(eLayerType::Monster);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"goomba");
		player->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/goomba/Animation/Jump.smd", 0.05f);
		animator->Play(L"test");*/
	}

	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-100.f, -100.f, 0.f));
		player->SetScale(Vector3(0.3f, 0.3f, 0.3f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Tank");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/Tank/Animation/Move.smd", 0.05f);
		animator->Play(L"test");*/
	}
	
	{

		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-100.f, -100.f, 0.f));
		player->SetScale(Vector3(0.3f, 0.3f, 0.3f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWomans");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/CityWomans/Animation/ByeBye.smd", 0.05f);
		animator->Play(L"test");*/

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

	// DebugTest
	{ 
		//deferred
		//{
		//	gameobj* test2 = object::instantiate<gameobj>(elayertype::objects);
		//	test2->setpos(vector3(-10.f, 5.f, 0.f));
		//	test2->setscale({ 5.f, 5.f, 5.f });
		//	test2->setname(l"test2");

		//	test2->addcomponent<meshrenderer>(ecomponenttype::meshrenderer);
		//	meshrenderer* testrender = test2->getcomponent<meshrenderer>();
		//	material* testmaterial = getsingle(resourcemgr)->find<material>(l"deferredmaterial");

		//	testrender->setmaterial(testmaterial);
		//	testrender->setmeshbykey(l"cubemesh");
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

	Panal* testObj = object::Instantiate<Panal>(eLayerType::Objects);
	testObj->SetPos(Vector3(0.f, 5.f, 0.f));
	testObj->SetScale({ 5.f, 5.f, 5.f });
	Material* testMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"SpriteMaterial");
	Texture* titleTexture = (GETSINGLE(ResourceMgr)->Find<Texture>(L"MarioTitle"));
	testMaterial->SetTexture(titleTexture);
	testObj->GetComponent <SpriteRenderer>()->SetMaterial(testMaterial);
	testObj->GetComponent<SpriteRenderer>()->SetMaterialByKey(L"SpriteMaterial");
	testObj->GetComponent<SpriteRenderer>()->SetMeshByKey(L"Rectmesh");


=======
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d

	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::CreateMainMenu()
{
	MainMenuPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 1.f), Vector3(1.0f, 1.0f, 1.0f), L"WorldMapPanal", this));

	HUD* worldMap = (GETSINGLE(UIFactory)->CreateHud(L"WorldMap", L"WorldMapMaterial", Vector3(0.f, 0.6f, 0.f), Vector3(3.f, 3.f, 1.f), MainMenuPanal, this));
	HUD* filter = (GETSINGLE(UIFactory)->CreateHud(L"RedFilter", L"FilterMaterial", Vector3(0.f, 0.6f, 0.f), Vector3(3.f, 3.f, 1.f), MainMenuPanal, this));
	worldMap->SetState(HUDState::Rotate);
	worldMap->SetSpeed(1);
	HUD* title = (GETSINGLE(UIFactory)->CreateHud(L"MarioTitle", L"TitleMaterial", Vector3(-0.6f, 0.3f, 0.f), Vector3(0.2f, 0.2f, 1.f), MainMenuPanal, this));
	HUD* bar = (GETSINGLE(UIFactory)->CreateHud(L"UIBar", L"UIBarMaterial", Vector3(-0.4f, 0.f, 0.f), Vector3(0.6f, 0.08f, 1.f), MainMenuPanal, this));
	bar->SetRotation(Vector3(0.0f, 0.0f, 2.0f));
	bar->SetState(HUDState::MoveBlink);
	HUD* cap = (GETSINGLE(UIFactory)->CreateHud(L"Cap", L"CapMaterial", Vector3(-0.35f, 0.0f, 0.f), Vector3(0.15f, 0.45f, 1.f), bar, this));
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
