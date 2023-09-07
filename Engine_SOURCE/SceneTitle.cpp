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


	if (KEY_TAP(N_1))
	{
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Play);
		return;
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
	renderer::mainCamera = mCamera->GetComponent<Camera>();
	mCamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mCamera->SetRotation(Vector3::Zero);
	mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mUICamera->SetRotation(Vector3::Zero);

	{
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLight");
		directionalLight->SetRotation(Vector3(45.f, -45.f, 0.f));
		directionalLight->SetScale(Vector3(15.f, 15.f, 15.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
	}

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
