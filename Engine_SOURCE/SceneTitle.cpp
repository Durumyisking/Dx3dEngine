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
		// UI Camera
		mUICamera = object::Instantiate<GameObj>(eLayerType::Camera, this, L"UICamera");
		mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
		Camera* cameraUIComp = mUICamera->AddComponent<Camera>(eComponentType::Camera);

		cameraUIComp->SetProjectionType(eProjectionType::Orthographic);
		cameraUIComp->SmoothOn();
		cameraUIComp->DisableLayerMasks();
		cameraUIComp->SetLayerMaskOn(eLayerType::UI);
		mUICamera->DontDestroy();

		renderer::UICamera = cameraUIComp;

		mCamera = object::Instantiate<GameObj>(eLayerType::Camera,this, L"MainCamera");
		mCamera->SetPos(Vector3(0.f, 5.f, -20.f));
		mCamera->DontDestroy();

		Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
		cameraComp->SmoothOn();
		cameraComp->SetProjectionType(eProjectionType::Perspective);
		cameraComp->SetLayerMaskOFF(eLayerType::UI);
		cameraComp->SetLayerMaskOFF(eLayerType::Camera);
		cameraComp->SetNear(0.01f);

		CameraScript* cameraScript = mCamera->AddComponent<CameraScript>(eComponentType::Script);
		cameraScript->SetUICameraObject(mUICamera);

		renderer::mainCamera = cameraComp;
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
	//mCamera->SetPos(Vector3(0.f, 5.f, -20.f));
	//mCamera->SetRotation(Vector3::Zero);
	//mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	//mUICamera->SetRotation(Vector3::Zero);
	//renderer::mainCamera = mCamera->GetComponent<Camera>();

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
	MainMenuPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(1.0f, 1.0f, 1.0f), L"WorldMapPanal", this));

	HUD* worldMap = (GETSINGLE(UIFactory)->CreateHud(L"WorldMap", L"WorldMapMaterial", Vector3(0.f, 450.0f, 100.f), Vector3(2000.f, 2000.f, 1.f), MainMenuPanal, this));
	HUD* filter = (GETSINGLE(UIFactory)->CreateHud(L"RedFilter", L"FilterMaterial", Vector3(0.f, 450.0f, 99.f), Vector3(2000.f, 2000.f, 1.f), MainMenuPanal, this));
	worldMap->SetState(HUDState::Rotate);
	worldMap->SetSpeed(1);
	HUD* title = (GETSINGLE(UIFactory)->CreateHud(L"MarioTitle", L"TitleMaterial", Vector3(-650.f, 300.f, 98.f), Vector3(200.f, 200.f, 1.f), MainMenuPanal, this));
	HUD* bar = (GETSINGLE(UIFactory)->CreateHud(L"UIBar", L"UIBarMaterial", Vector3(-500.f, 0.f, 98.f), Vector3(600.f, 100.f, 1.f), MainMenuPanal, this));
	bar->SetRotation(Vector3(0.0f, 0.0f, 2.0f));
	bar->SetState(HUDState::MoveBlink);
	HUD* cap = (GETSINGLE(UIFactory)->CreateHud(L"Cap", L"CapMaterial", Vector3(-0.3f, 0.f, -1.f), Vector3(0.2f, 0.6f, 1.f), bar, this));
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
