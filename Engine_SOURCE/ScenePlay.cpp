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

void ScenePlay::Initialize()
{
	CreateCameras();

	//{
	//	GameObj* dirLight = object::Instantiate<GameObj>(eLayerType::Player, this);
	//	dirLight->SetPos(Vector3(1000.f, 1000.f, 0.));
	//	dirLight->SetScale(Vector3(1.f, 1.f, 1.f));
	//	Light* light = dirLight->AddComponent<Light>(eComponentType::Light);
	//	light->SetType(eLightType::Directional);
	//	light->SetAngle(45.f);
	//}

	//CityWorld Model Test
	{
		{
			GameObj* obj = object::Instantiate<GameObj>(eLayerType::Platforms, this);
			obj->SetPos(Vector3(0.f, -10.f, 300.f));
			obj->SetScale(Vector3(0.03f, 0.03f, 0.03f));
			obj->SetName(L"CityWorldHomeGroundCollider");

			// SetModel
			Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorld_RoadGroundCollider");
			if (model)
			{
				MeshRenderer* test = obj->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
				test->SetModel(model, model->GetMaterial(0));
			}

			Physical* physical = obj->AddComponent<Physical>(eComponentType::Physical);
			//physical->InitialConvexMeshProperties(eActorType::Static, Vector3(0.03f, 0.03f, 0.03f));
			physical->InitialTriangleMeshProperties(Vector3(0.03f, 0.03f, 0.03f));

			PhysXRigidBody* rigid = obj->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
			obj->AddComponent<PhysXCollider>(eComponentType::Collider);
		}
	}

	//ConvexMesh Test
	{
		GameObj* ball = object::Instantiate<GameObj>(eLayerType::Monster, this);
		ball->SetPos(Vector3(0.f, 50.f, 150.f));
		//ball->SetScale(Vector3(0.2f, 0.2f, 0.2f));
		ball->SetScale(Vector3(1.f, 1.f, 1.f));
		ball->SetName(L"PackunBall");

		// SetModel
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"PackunBall");
		if (model)
		{
			MeshRenderer* test = ball->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
			test->SetModel(model, model->GetMaterial(0));
		}

		Physical* physical = ball->AddComponent<Physical>(eComponentType::Physical);
		//physical->InitialConvexMeshProperties(eActorType::Dynamic, Vector3(1.f, 1.f, 1.f));
		physical->InitialTriangleMeshProperties(Vector3(1.f, 1.f, 1.f));

		PhysXRigidBody* rigid = ball->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		ball->AddComponent<PhysXCollider>(eComponentType::Collider);
	}


	{
		Player* player = object::Instantiate<Player>(eLayerType::Monster, this);
		player->SetPos(Vector3(-30.f, 55.f, -9.5f));
		player->SetScale(Vector3(5.f, 5.f, 5.f));
		player->SetName(L"Player");

		player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PBRMaterial");
		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
		//player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(5.f, 5.f, 5.f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		//player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);

	}

	{
		Player* Sphere = object::Instantiate<Player>(eLayerType::Player, this);
		Sphere->SetPos(Vector3(32.f, 55.f, -9.5f));
		Sphere->SetScale(Vector3(10.f, 10.f, 10.f));
		Sphere->SetName(L"Player");
		Sphere->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
		Sphere->GetComponent<MeshRenderer>()->GetMaterial()->SetMetallic(0.99f);
		Sphere->GetComponent<MeshRenderer>()->GetMaterial()->SetRoughness(0.01f);

		Sphere->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		Sphere->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = Sphere->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(5.f, 5.f, 5.f));

		PhysXRigidBody* rigid = Sphere->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		Sphere->AddComponent<PhysXCollider>(eComponentType::Collider);
		//player->AddComponent<PhysicalMovement>(eComponentType::Movement);
		Sphere->AddComponent<PlayerScript>(eComponentType::Script);
	}

	//	PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

	//	player->AddComponent<PhysXCollider>(eComponentType::Collider);
	//	player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	//}


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
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"DeferredMaterial");
		plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		rigid->RemoveGravity();

	//	//plane->AddComponent<PhysXCollider>(eComponentType::Collider);
	//}

	//{
	//	Packun* packun = object::Instantiate<Packun>(eLayerType::Monster, this);
	//}

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
		HUD* hud = (GETSINGLE(UIFactory)->CreateHud(L"LifeGauge", L"LifeGauge_3Material", Vector3(7.f, 3.6f, 0.f), Vector3::One, mLifePanal, this));
		//Lifeheart
		HUD* lifeheart = (GETSINGLE(UIFactory)->CreateHud(L"LifeHeart", L"LifeheartMaterial", Vector3(7.f, 3.55f, 0.f), Vector3(0.6f, 0.6f, 1.0f), mLifePanal, this));

		ImageUI* lifeText = (GETSINGLE(UIFactory)->CreateImage(L"LifeText", L"LifeTextMaterial", Vector3(7.f, 3.55f, -0.1f), Vector3(0.4f, 0.4f, 1.f), mLifePanal, this));
		lifeText->SetColor(Vector4(0.1f, 0.1f, 0.1f, 1.0f), true);

		mLifePanal->Addchild(hud);
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
}