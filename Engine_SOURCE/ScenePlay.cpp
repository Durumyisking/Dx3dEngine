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
#include "LifeUI.h"
#include "CompassUI.h"

#include "Goomba.h"
#include "Packun.h"
#include "ModelObj.h"

ScenePlay::ScenePlay()
	: mCoinPanal(nullptr)
	, mLifePanal(nullptr)
	, mLunaPanal(nullptr)
	, mCoinTextPanal(nullptr)
	, mLunaTextPanal(nullptr)
{
}

ScenePlay::~ScenePlay()
{
}

bool ScenePlay::Save()
{
	return false;
}

bool ScenePlay::Load()
{
	return false;
}

void ScenePlay::Initialize()
{
	CreateCameras();

	if (GetType() == SceneMgr::eSceneType::Test)
	{
		{
			SkySphere* skySphere = object::Instantiate<SkySphere>(eLayerType::SkySphere, this);
			skySphere->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			skySphere->SetName(L"SkySphere");
		}

		CreatePlayerUI();
		Scene::Initialize();

		return;
	}

	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Platforms, eLayerType::Player);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Platforms, eLayerType::Monster);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Player, eLayerType::Monster);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Objects, eLayerType::Monster);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Monster, eLayerType::Platforms);
	//Convex and Triangle Mesh TEST
	{
		//TriangleMesh Test
		{
			ModelObj* obj = object::Instantiate<ModelObj>(eLayerType::Platforms, this);
			obj->SetPos(Vector3(0.f, -5.f, 300.f));
			obj->SetScale(Vector3(0.06f, 0.06f, 0.06f));
			obj->SetName(L"CityWorld_NaviCollider");

			obj->SetModel(L"CityWorld_NaviRoadCollider");
			obj->SetPhysical(true, eGeometryType::TriangleMesh, math::Vector3(0.06f, 0.06f, 0.06f));
		}


		////Sphere 
		//{
		//	GameObj* Sphere = object::Instantiate<GameObj>(eLayerType::Player, this);
		//	Sphere->SetPos(Vector3(32.f, 25.f, -9.5f));
		//	Sphere->SetScale(Vector3(15.f, 15.f, 15.f));
		//	Sphere->SetName(L"Sphere");
		//
		//
		//	Sphere->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		//	Sphere->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
		//	Sphere->GetComponent<MeshRenderer>()->GetMaterial()->SetMetallic(0.99f);
		//	Sphere->GetComponent<MeshRenderer>()->GetMaterial()->SetRoughness(0.01f);
		//
		//	Sphere->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		//
		//	Physical* physical = Sphere->AddComponent<Physical>(eComponentType::Physical);
		//	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Capsule, Vector3(7.5f, 7.5f, 7.5f));
		//
		//	PhysXRigidBody* rigid = Sphere->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		//
		//	Sphere->AddComponent<PhysXCollider>(eComponentType::Collider);
		//	//Sphere->AddComponent<PhysicalMovement>(eComponentType::Movement);
		//	Sphere->AddComponent<PlayerScript>(eComponentType::Script);
		//}
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
		//GameObj* plane = object::Instantiate<GameObj>(eLayerType::Platforms, this);
		//plane->SetPos(Vector3(0.f, -0.251f, 0.f));
		//plane->SetScale({ 1000.f, 0.5f, 1000.f });
		//plane->SetName(L"Plane");
		//plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"DeferredMaterial");
		//plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		//PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		//rigid->RemoveGravity();

		//plane->AddComponent<PhysXCollider>(eComponentType::Collider);
	}

	{
		MarioCap* mariocap = object::Instantiate<MarioCap>(eLayerType::Player, this);
		Player* player = object::Instantiate<Player>(eLayerType::Player, this);
		player->SetMarioCap(mariocap);
	}

	{
		Goomba* goomba = object::Instantiate<Goomba>(eLayerType::Monster, this);
		goomba->SetPos(Vector3(5.f, 0.f, 0.f));
	}

	{
		Goomba* packun = object::Instantiate<Goomba>(eLayerType::Monster, this);
		packun->SetPos({ 15.f, 5.f, 15.f });
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

	mCamera->SetPos(Vector3(0.f, 150.f, -150.f));
	mCamera->GetComponent<Transform>()->SetRotationX(45.f);
}

void ScenePlay::Exit()
{
	Scene::Exit();
}

void ScenePlay::CreatePlayerUI()
{
	//Life UI
	{
		mLifePanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 10.f), Vector3(100.0f, 100.0f, 1.0f), L"LifePanal", this, eUIType::HP));
		//LifeGauge
		LifeUI* gauge = (GETSINGLE(UIFactory)->CreateUI<LifeUI>(L"LifeGauge", L"LifeGauge_3Material", eUIType::None, Vector3(7.f, 3.6f, 0.f), Vector3::One, mLifePanal, this));
		gauge->SetUIActive();
		gauge->SetTargetPos(Vector3::Zero);
		//Lifeheart
		LifeUI* lifeheart = (GETSINGLE(UIFactory)->CreateUI<LifeUI>(L"LifeHeart", L"LifeheartMaterial", eUIType::None, Vector3(7.f, 3.55f, 0.f), Vector3(0.6f, 0.6f, 1.0f), mLifePanal, this));
		lifeheart->SetUIActive();
		lifeheart->SetTargetPos(Vector3(0.0f, -0.05f, 0.0f));

		LifeUI* lifeText = (GETSINGLE(UIFactory)->CreateUI<LifeUI>(L"LifeText", L"LifeTextMaterial", eUIType::HPText, Vector3(7.f, 3.55f, -0.1f), Vector3(0.4f, 0.4f, 1.f), mLifePanal, this));
		lifeText->InitColor(Vector4(0.1f, 0.1f, 0.1f, 1.0f));
		lifeText->SetUIActive();
		lifeText->SetTargetPos(Vector3(0.0f, -0.05f, -0.1f));


		mLifePanal->Addchild(gauge);
		mLifePanal->Addchild(lifeheart);
		mLifePanal->Addchild(lifeText);
	}

	//Left Coin UI
	{
		mCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.f, 0.f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CoinPanal", this, eUIType::Coin));
		mCoinTextPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.f, 0.f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CoinPanal", this, eUIType::CoinText));



		ImageUI* coin = (GETSINGLE(UIFactory)->CreateImage(L"Coin", L"CoinMaterial", Vector3(-7.f, 3.5f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal, this));
		coin->SetUIActive();
		ImageUI* cityCoin = (GETSINGLE(UIFactory)->CreateImage(L"CityCoin", L"CityCoinMaterial", Vector3(-5.f, 3.6f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal, this));
		cityCoin->SetUIActive();
		ImageUI* bar = (GETSINGLE(UIFactory)->CreateImage(L"Bar", L"BarMaterial", Vector3(-5.4f, 2.9f, 0.f), Vector3(4.2f, 1.4f, 1.f), mCoinPanal, this));
		mCoinPanal->Addchild(coin);
		mCoinPanal->Addchild(bar);

		for (size_t i = 0; i < 3; i++)
		{
			ImageUI* image = (GETSINGLE(UIFactory)->CreateImage(L"CoinText", L"CoinTextMaterial_" + std::to_wstring(i), Vector3(-6.35f + (0.34f * i), 3.3f, 0.f), Vector3(0.5f, 0.5f, 1.0f), mCoinTextPanal, this));
			image->SetUIActive();

			mCoinTextPanal->Addchild(image);
		}



		Panal* cityCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CityCoinPanal", this, eUIType::CityCoin));
		mLunaTextPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CityCoinTextPanal", this, eUIType::CityCoinText));

		cityCoinPanal->Addchild(cityCoin);
		for (size_t i = 0; i < 3; i++)
		{
			ImageUI* image = (GETSINGLE(UIFactory)->CreateImage(L"CoinText", L"CityCoinTextMaterial_" + std::to_wstring(i), Vector3(-4.35f + (0.34f * i), 3.3f, 0.f), Vector3(0.5f, 0.5f, 1.0f), mLunaTextPanal, this));
			image->SetUIActive();

			mLunaTextPanal->Addchild(image);
		}

	}

	//left Luna UI
	{
		mLunaPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"LunaPanal", this, eUIType::Luna));


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

	//CompassUI
	{
		//mCompassPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CompassPanal", this, eUIType::Compass));

		//CompassUI* compass = (GETSINGLE(UIFactory)->CreateUI<CompassUI>(L"Compass", L"CompassMaterial", eUIType::None, Vector3(7.f, 2.5f, 2.f), Vector3::One, mCompassPanal, this));
		//ImageUI* compassBar = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"CompassBar", L"CompassBarMaterial", eUIType::None, Vector3(7.f, 2.5f, -1.f), Vector3::One, mCompassPanal, this));
		//ImageUI* compassNeedle = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"CompassNeedle", L"CompassNeedleMaterial", eUIType::None, Vector3(7.f, 2.5f, -1.f), Vector3(2.0f,2.0f,1.0f), mCompassPanal, this));

		//mCompassPanal->Addchild(compass);
		//mCompassPanal->Addchild(compassBar);
		//mCompassPanal->Addchild(compassNeedle);
	}
}