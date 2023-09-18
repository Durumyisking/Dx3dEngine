#include "ScenePlay.h"
//#include "TimeMgr.h"
//#include "InputMgr.h"
//
//#include "Object.h"
//#include "Camera.h"
//#include "Transform.h"
//#include "SpriteRenderer.h"
//
//#include "Texture.h"
//#include "CameraScript.h"
//
//#include "Panal.h"
//#include "HUD.h"
//#include "Button.h"
//#include "UIFactory.h"
//#include "Animator.h"


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
#include "ImageUI.h"
#include "UIFactory.h"
#include "Animator.h"

#include "Goomba.h"
#include "Packun.h"


ScenePlay::ScenePlay()
	: mCamera(nullptr)
	, mUICamera(nullptr)
	, mCoinPanal(nullptr)
	, mLifePanal(nullptr)
	, mLunaPanal(nullptr)
	, mCoinTextPanal(nullptr)
	, mLunaTextPanal(nullptr)
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::Initialize()
{
	CreateCameras();



	{
		Player* player = object::Instantiate<Player>(eLayerType::Player, this);
		player->SetPos(Vector3(15.f, 10.f, 9.5f));
		player->SetScale(Vector3(1.f, 1.f, 1.f));
		player->SetName(L"Player");
		player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PBRMaterial");
		player->GetComponent<MeshRenderer>()->GetMaterial()->SetMetallic(0.99f);
		player->GetComponent<MeshRenderer>()->GetMaterial()->SetRoughness(0.01f);

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Capsule, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	{
		Goomba* goomba = object::Instantiate<Goomba>(eLayerType::Monster, this);
		goomba->SetPos(Vector3(0.f, 5.f, -17.f));
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Objects, this);
		player->SetPos(Vector3(-15.f, 10.f, 9.5f));
		player->SetScale(Vector3(1.f, 1.f, 1.f));
		player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"DeferredMaterial");

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
		//player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}


	{
		CubeMapHDR* cubeMap = object::Instantiate<CubeMapHDR>(eLayerType::CubeMap, this);

		Texture* t = GETSINGLE(ResourceMgr)->Find<Texture>(L"night11");
		t->BindAllShaderResource(12);
	}
	{
		SkySphere* skySphere = object::Instantiate<SkySphere>(eLayerType::SkySphere, this);
		skySphere->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		skySphere->SetName(L"SkySphere");
	}

	{
		GameObj* plane = object::Instantiate<GameObj>(eLayerType::Platforms, this);
		plane->SetPos(Vector3(0.f, -0.251f, 0.f));
		plane->SetScale({ 1000.f, 0.5f, 1000.f });
		plane->SetName(L"Plane");
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"PBRMaterial");
		plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		plane->AddComponent<PhysXCollider>(eComponentType::Collider);
	}


	CreatePlayerUI();

	Scene::Initialize();
}

void ScenePlay::update()
{
	if (KEY_TAP(N_1))
	{
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Title);
	}


	Scene::update();
}

void ScenePlay::fixedUpdate()
{

	Scene::fixedUpdate();
}

void ScenePlay::render()
{

}

void ScenePlay::Enter()
{

	Scene::Enter();
}

void ScenePlay::Exit()
{
	Scene::Exit();
}

void ScenePlay::CreatePlayerUI()
{
	//Life UI
	{
		mLifePanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 10.f), Vector3(100.0f, 100.0f, 1.0f), L"LifePanal", this));
		//LifeGauge
		HUD* hud = (GETSINGLE(UIFactory)->CreateHud(L"LifeGauge", L"LifeGauge_3Material", Vector3(7.f, 3.6f, 0.f), Vector3::One, mLifePanal, this));
		//Lifeheart
		HUD* lifeheart = (GETSINGLE(UIFactory)->CreateHud(L"LifeHeart", L"LifeheartMaterial", Vector3(7.f, 3.55f, 0.f), Vector3(0.6f, 0.6f, 1.0f), mLifePanal, this));

		mLifePanal->Addchild(hud);
		mLifePanal->Addchild(lifeheart);
	}

	//Left Coin UI
	{
		mCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.f, 0.f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CoinPanal", this));
		mCoinTextPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.f, 0.f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CoinPanal", this));

		HUD* coin = (GETSINGLE(UIFactory)->CreateHud(L"Coin", L"CoinMaterial", Vector3(-7.f, 3.5f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal, this));
		HUD* cityCoin = (GETSINGLE(UIFactory)->CreateHud(L"CityCoin", L"CityCoinMaterial", Vector3(-5.f, 3.6f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal, this));
		ImageUI* bar = (GETSINGLE(UIFactory)->CreateImage(L"Bar", L"BarMaterial", Vector3(-5.4f, 2.9f, 0.f), Vector3(4.2f, 1.4f, 1.f), mCoinPanal, this));
		mCoinPanal->Addchild(coin);
		mCoinPanal->Addchild(cityCoin);
		mCoinPanal->Addchild(bar);
	}

	//left Luna UI
	{
		mLunaPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"LunaPanal", this));
		mLunaTextPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"LunaTextPanal", this));

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				Vector3 position = Vector3(-7.f + (0.33f * i), (2.5f - (0.35f * j)) + (0.01f * i), 0.f);


				ImageUI* luna = (GETSINGLE(UIFactory)->CreateImage(L"Luna" + std::to_wstring(i) + std::to_wstring(j * 10), L"LunaMaterial",
					position,
					Vector3(0.3f, 0.3f, 1.f),
					mLunaPanal, this));
				luna->Die();
				mLunaPanal->Addchild(luna);

				ImageUI* dottedLine = (GETSINGLE(UIFactory)->CreateImage(L"DottedLine" + std::to_wstring(i) + std::to_wstring(j * 10), L"DottedLineMaterial",
					position,
					Vector3(0.3f, 0.3f, 1.f),
					mLunaPanal, this));

				mLunaPanal->Addchild(dottedLine);
			}
		}
	}
}