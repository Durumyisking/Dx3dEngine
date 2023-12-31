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
#include "ImageUI.h"
#include "CapUI.h"
#include "CapEyeUI.h"

#include "WorldMapScript.h"
#include "TitleUIBarScript.h"
#include "CapMoveScript.h"
#include "CapEyeScript.h"
#include "AudioSource.h"
#include "ResourceMgr.h"
#include "AudioClip.h"
#include "AudioListener.h"
#include "TitleBGMScript.h"

extern Application application;


SceneTitle::SceneTitle()
	:mMainMenuPanal{}
	,mBarPanal{}

{
	SetName(L"SceneTitle");
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Save(FILE* File)
{
}

void SceneTitle::Load(FILE* File)
{
}

void SceneTitle::Initialize()
{
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Platforms, eLayerType::Player);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Platforms, eLayerType::Monster);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Monster, eLayerType::Player);

	CreateCameras();
	CreateMainMenu();


	Scene::Initialize();
}

void SceneTitle::update()
{
	if (KEY_TAP(F_9))
	{
		GETSINGLE(server::ServerMgr)->OpenServer();
	}
	if (KEY_TAP(F_10))
	{
		GETSINGLE(server::ServerMgr)->ConnectAsClient();
	}
	if (KEY_TAP(F_1))
	{
		mBarPanal->GetChilds()[0]->GetScript<TitleUIBarScript>()->SelectUI();
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
	Scene::Enter();
}

void SceneTitle::Exit()
{
	mMainMenuPanal->GetComponent<AudioSource>()->Stop(L"TitleBGM");

	Scene::Exit();
}

void SceneTitle::CreateMainMenu()
{
	{
		mMainMenuPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"WorldMapPanal", eUIType::MainMenu));
		mMainMenuPanal->AddComponent<AudioSource>(eComponentType::AudioSource)->AddClipByKey(L"TitleBGM");
		mMainMenuPanal->GetComponent<AudioSource>()->AddClipByKey(L"SuperMarioOdyssey");
		mMainMenuPanal->AddComponent<TitleBGMScript>(eComponentType::Script)->SetStartSound();

		ImageUI* worldMap = (GETSINGLE(UIFactory)->CreateImage(L"WorldMap", L"WorldMapMaterial", Vector3(0.f, 4.5f, 100.f), Vector3(20.f, 20.f, 1.f), mMainMenuPanal, this));
		worldMap->AddComponent<WorldMapScript>(eComponentType::Script);
		ImageUI* filter = (GETSINGLE(UIFactory)->CreateImage(L"RedFilter", L"FilterMaterial", Vector3(0.f, 4.5f, 99.f), Vector3(20.f, 20.f, 1.f), mMainMenuPanal, this));
		ImageUI* title = (GETSINGLE(UIFactory)->CreateImage(L"MarioTitle", L"TitleMaterial", Vector3(-6.5f, 3.f, 98.f), Vector3(2.f, 2.f, 1.f), mMainMenuPanal, this));

		mMainMenuPanal->Addchild(worldMap);
		mMainMenuPanal->Addchild(filter);
		mMainMenuPanal->Addchild(title);
	}

	{
		mBarPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"Start2PTextPanal", eUIType::TitleImg));

		ImageUI* bar = (GETSINGLE(UIFactory)->CreateImage(L"UIBar", L"UIBarMaterial", Vector3(-5.f, 0.f, 98.f), Vector3(6.f, 1.f, 1.f), mBarPanal, this));
		bar->AddComponent<AudioSource>(eComponentType::AudioSource)->AddClipByKey(L"UIBarMove");
		bar->GetComponent<AudioSource>()->AddClipByKey(L"GameStartSoundEffect");
		bar->AddComponent<TitleUIBarScript>(eComponentType::Script);
		//bar->SetColor(); 
		bar->SetRotation(Vector3(0.0f, 0.0f, 2.0f));

		ImageUI* cap = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"Cap", L"CapMaterial", eUIType::None, Vector3(-0.33f, 0.0f, -0.1f), Vector3(0.2f, 0.6f, 1.f), bar, this));
		cap->AddComponent<CapMoveScript>(eComponentType::Script);
		Animator* capAni = cap->AddComponent<Animator>(eComponentType::Animator);
		Texture* capRotTexture = (GETSINGLE(ResourceMgr)->Find<Texture>(L"CapRotate"));
		capAni->Create(L"CapRotate", capRotTexture, Vector2::Zero, Vector2(84.0f, 60.0f), Vector2::One, 5, Vector2(80.0f, 80.0f), 0.1f);

		ImageUI* capEye = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"CapEye", L"CapMaterial", eUIType::None, Vector3(0.2f, 0.2f, -0.2f), Vector3(0.f, 0.f, 1.f), cap, this));
		capEye->AddComponent<CapEyeScript>(eComponentType::Script);
		Animator* capEyeAni = capEye->AddComponent<Animator>(eComponentType::Animator);
		Texture* capEyeTexture = (GETSINGLE(ResourceMgr)->Find<Texture>(L"CapEye"));

		capEyeAni->Create(L"CapEyeAni", capEyeTexture, Vector2::Zero, Vector2(40.0f, 40.0f), Vector2::One, 8, Vector2(80.0f, 80.0f), 0.1f);
		capEyeAni->Play(L"CapEyeAni", false);

		mBarPanal->Addchild(bar);
		mBarPanal->Addchild(cap);

	}


	{
		Panal* mStartTextPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"StartTextPanal", eUIType::TitleText));

		ImageUI* start = (GETSINGLE(UIFactory)->CreateImage(L"Start", L"StartMaterial", Vector3(-5.5f, 0.f, 97.0f), Vector3(1.4f, 0.8f, 1.f), mStartTextPanal, this));
		start->SetRotation(Vector3(0.0f, 0.0f, 1.0f));
		start->SetColor(Vector4(0.4f, 0.4f, 0.4f, 1.0f), true);

		ImageUI* resume = (GETSINGLE(UIFactory)->CreateImage(L"Resume", L"ResumeMaterial", Vector3(-5.4f, -1.f, 97.0f), Vector3(2.f, 0.8f, 1.f), mStartTextPanal, this));
		resume->SetRotation(Vector3(0.0f, 0.0f, 1.0f));

		ImageUI* resume2P = (GETSINGLE(UIFactory)->CreateImage(L"Resume2P", L"Resume2PMaterial", Vector3(-5.2f, -2.f, 97.0f), Vector3(2.f, 0.8f, 1.f), mStartTextPanal, this));
		resume2P->SetRotation(Vector3(0.0f, 0.0f, 1.0f));

		ImageUI* exit = (GETSINGLE(UIFactory)->CreateImage(L"Exit", L"ExitMaterial", Vector3(-5.55f, -3.f, 97.0f), Vector3(2.f, 0.8f, 1.f), mStartTextPanal, this));
		exit->SetRotation(Vector3(0.0f, 0.0f, 1.0f));


		mStartTextPanal->Addchild(start);
		mStartTextPanal->Addchild(resume);
		mStartTextPanal->Addchild(resume2P);
		mStartTextPanal->Addchild(exit);
	}
}