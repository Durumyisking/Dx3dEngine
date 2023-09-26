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

#include "ImageUI.h"
#include "CapUI.h"
#include "CapEyeUI.h"



extern Application application;


SceneTitle::SceneTitle()

{
}

SceneTitle::~SceneTitle()
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
	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::CreateMainMenu()
{
	{
		Panal* mMainMenuPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"WorldMapPanal", this, eUIType::TitleImg));
		HUD* worldMap = (GETSINGLE(UIFactory)->CreateHud(L"WorldMap", L"WorldMapMaterial", Vector3(0.f, 4.5f, 100.f), Vector3(20.f, 20.f, 1.f), mMainMenuPanal, this));
		//worldMap->SetColor();
		ImageUI* filter = (GETSINGLE(UIFactory)->CreateImage(L"RedFilter", L"FilterMaterial", Vector3(0.f, 4.5f, 99.f), Vector3(20.f, 20.f, 1.f), mMainMenuPanal, this));
		//filter->SetColor();
		worldMap->SetState(eHUDState::Rotate);
		worldMap->SetSpeed(1);
		ImageUI* title = (GETSINGLE(UIFactory)->CreateImage(L"MarioTitle", L"TitleMaterial", Vector3(-6.5f, 3.f, 98.f), Vector3(2.f, 2.f, 1.f), mMainMenuPanal, this));

		mMainMenuPanal->Addchild(worldMap);
		mMainMenuPanal->Addchild(filter);
		mMainMenuPanal->Addchild(title);
	}

	{
		Panal* mBarPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"Start2PTextPanal", this, eUIType::Bar));

		HUD* bar = (GETSINGLE(UIFactory)->CreateHud(L"UIBar", L"UIBarMaterial", Vector3(-5.f, 0.f, 98.f), Vector3(6.f, 1.f, 1.f), mBarPanal, this));
		//bar->SetColor();
		bar->SetRotation(Vector3(0.0f, 0.0f, 2.0f));
		bar->SetChangeSize(Vector3(1.f, 0.9f, 1.0f));
		bar->SetState(eHUDState::MoveBlink);

		CapUI* cap = (GETSINGLE(UIFactory)->CreateUI<CapUI>(L"Cap", L"CapMaterial", eUIType::TitleImg, Vector3(-0.33f, 0.0f, -0.1f), Vector3(0.2f, 0.6f, 1.f), bar, this));
		//cap->SetColor();
		cap->SetState(eHUDState::TitleCapMove);
		cap->SetTargetPos(cap->GetComponent<Transform>()->GetPosition() + Vector3(0.05f, 0.0f, 0.0f));

		CapEyeUI* capEye = (GETSINGLE(UIFactory)->CreateUI<CapEyeUI>(L"CapEye", L"CapMaterial", eUIType::TitleImg, Vector3(-0.3f, 0.1f, -0.2f), Vector3(0.2f, 1.f, 1.f), bar, this));
		capEye->SetTargetPos(capEye->GetComponent<Transform>()->GetPosition() + Vector3(0.05f, 0.0f, 0.0f));
		//capEye->SetColor(); 
		Animator* capEyeAni = capEye->AddComponent<Animator>(eComponentType::Animator);
		Texture* capEyeTexture = (GETSINGLE(ResourceMgr)->Find<Texture>(L"CapEye"));
		capEyeAni->Create(L"CapEyeAni", capEyeTexture, Vector2::Zero, Vector2(40.0f, 40.0f), Vector2::One, 8, Vector2(80.0f, 80.0f), 0.1f);
		capEyeAni->Play(L"CapEyeAni", false);

		mBarPanal->Addchild(bar);
		mBarPanal->Addchild(cap);

	}


	{
		Panal* mStartTextPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"StartTextPanal", this, eUIType::TitleText));

		ImageUI* start = (GETSINGLE(UIFactory)->CreateImage(L"Start", L"StartMaterial", Vector3(-5.5f, 0.f, 97.0f), Vector3(1.4f, 0.8f, 1.f), mStartTextPanal, this));
		start->SetRotation(Vector3(0.0f, 0.0f, 1.0f));
		start->SetColor(Vector4(0.4f, 0.4f, 0.4f, 1.0f), true);

		ImageUI* resume = (GETSINGLE(UIFactory)->CreateImage(L"Resume", L"ResumeMaterial", Vector3(-5.4f, -1.f, 97.0f), Vector3(2.f, 0.8f, 1.f), mStartTextPanal, this));
		resume->SetRotation(Vector3(0.0f, 0.0f, 1.0f));

		ImageUI* resume2P = (GETSINGLE(UIFactory)->CreateImage(L"Resume2P", L"Resume2PMaterial", Vector3(-5.2f, -2.f, 97.0f), Vector3(2.f, 0.8f, 1.f), mStartTextPanal, this));
		resume2P->SetRotation(Vector3(0.0f, 0.0f, 1.0f));

		ImageUI* exit = (GETSINGLE(UIFactory)->CreateImage(L"Exit", L"ExitMaterial", Vector3(-5.55f, -3.f, 97.0f), Vector3(2.f, 0.8f, 1.f), mStartTextPanal, this));
		exit->SetRotation(Vector3(0.0f, 0.0f, 1.0f));


		mStartTextPanal->Addchild(exit);
		mStartTextPanal->Addchild(resume2P);
		mStartTextPanal->Addchild(resume);
		mStartTextPanal->Addchild(start);
	}
	////HUD* zero = (GETSINGLE(UIFactory)->CreateHud(L"S", L"uppercase_18Material", Vector3(0.f, 0.f, 97.f), Vector3(0.8f, 0.8f, 1.f), MainMenuPanal, this));
	//(GETSINGLE(UIFactory)->CreateString(L"Start", Vector3(-6.0f,0.0f,1.0f), Vector3(0.5f, 0.5f, 1.0f), 0.3f, mStartTextPanal, this));
	//(GETSINGLE(UIFactory)->CreateString(L"2P Mode", Vector3(-6.0f, -1.0f, 1.0f), Vector3(0.5f, 0.5f, 1.0f), 0.3f, mStart2PTextPanal, this));
	//(GETSINGLE(UIFactory)->CreateString(L"End", Vector3(-6.0f, -2.0f, 1.0f), Vector3(0.5f, 0.5f, 1.0f), 0.3f, mExitTextPanal, this));

	//for (size_t i = 0; i < mStartTextPanal->GetChilds().size(); i++)
	//{
	//	mStartTextPanal->GetChilds()[i]->SetColor(Vector4(0.5f,0.5f,0.5f,1.0f),true);
	//}


	////Animator* capAni = cap->AddComponent<Animator>(eComponentType::Animator);
	////Texture* tex = (GETSINGLE(ResourceMgr)->Find<Texture>(L"CapRotate"));
	////capAni->Create(L"CapAni", tex, Vector2::Zero, Vector2(84.0f, 50.0f), Vector2::One, 5, Vector2(100.0f, 80.0f), 0.1f);
	////capAni->Play(L"CapAni", true);
}