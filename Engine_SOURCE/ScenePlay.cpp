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
#include "UIFactory.h"
#include "Animator.h"

#include "Packun.h"
#include "Goomba.h"
#include "PackunPostionBall.h"



ScenePlay::ScenePlay()
	: mCamera(nullptr)
	, mUICamera(nullptr)
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::Initialize()
{
	{
		mCamera = object::Instantiate<GameObj>(eLayerType::Camera, this, L"MainCamera");
		Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraComp->SmoothOn();
		mCamera->AddComponent<CameraScript>(eComponentType::Script);
		cameraComp->SetProjectionType(eProjectionType::Perspective);
		mCamera->SetPos(Vector3(0.f, 5.f, -20.f));

	}

	{
		// UI Camera
		mUICamera = object::Instantiate<GameObj>(eLayerType::Camera, this, L"UICamera");
		Camera* cameraUIComp = mUICamera->AddComponent<Camera>(eComponentType::Camera);
		mUICamera->AddComponent<CameraScript>(eComponentType::Script);

		cameraUIComp->SetProjectionType(eProjectionType::Perspective);
		cameraUIComp->SmoothOn();
		cameraUIComp->DisableLayerMasks();
		cameraUIComp->TurnLayerMask(eLayerType::UI, true);
		mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	}


	{
		Player* player = object::Instantiate<Player>(eLayerType::Player, this);
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
		//player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player, this);
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

		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Capsule, Vector3(0.5f, 1.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player, this);
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
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"DeferredMaterial");
		plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		plane->AddComponent<PhysXCollider>(eComponentType::Collider);
	}


	{
		Packun* packun = object::Instantiate<Packun>(eLayerType::Monster,this);
		//PackunPostionBall* packunball = object::Instantiate<PackunPostionBall>(eLayerType::Objects, this);

		//Goomba* goomba = object::Instantiate<Goomba>(eLayerType::Monster, this);
	}


	CreatePlayerUI();

	Scene::Initialize();
}

void ScenePlay::update()
{
	if (KEY_TAP(N_1))
	{
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Title);
		return;
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
	mCamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mCamera->SetRotation(Vector3::Zero);
	mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mUICamera->SetRotation(Vector3::Zero);
	renderer::mainCamera = mCamera->GetComponent<Camera>();

	{
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLight");
		directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 1000.f, 0.f));
		directionalLight->SetRotation(Vector3(45.f, 0.f, 0.f));
		directionalLight->SetScale(Vector3(15.f, 15.f, 15.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
	}

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
		mLifePanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(6.4f, 3.1f, 10.f), Vector3(1.0f, 1.0f, 1.0f), L"LifePanal",this));
		//LifeGauge
		HUD* hud = (GETSINGLE(UIFactory)->CreateHud(L"LifeGauge", L"LifeGauge_3Material", Vector3(0.f, 0.f, 0.f), Vector3::One, mLifePanal, this));
		//Lifeheart
		HUD* lifeheart = (GETSINGLE(UIFactory)->CreateHud(L"LifeHeart", L"LifeheartMaterial", Vector3(0.f, -0.03f, 0.f), Vector3(0.6f, 0.6f, 1.0f), mLifePanal, this));

		mLifePanal->Addchild(hud);
		mLifePanal->Addchild(lifeheart);
	}

	//Left Coin UI
	{
		mCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(-6.4f, 3.1f, 10.f), Vector3(0.7f, 0.7f, 1.0f), L"CoinPanal", this));
		HUD* coin = (GETSINGLE(UIFactory)->CreateHud(L"Coin", L"CoinMaterial", Vector3(0.f, 0.f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal, this));
		HUD* cityCoin = (GETSINGLE(UIFactory)->CreateHud(L"CityCoin", L"CityCoinMaterial", Vector3(3.f, 0.1f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal, this));
		HUD* bar = (GETSINGLE(UIFactory)->CreateHud(L"Bar", L"BarMaterial", Vector3(2.f, -0.55f, 0.f), Vector3(5.f, 1.4f, 1.f), mCoinPanal, this));
		mCoinPanal->Addchild(coin);
		mCoinPanal->Addchild(cityCoin);
		mCoinPanal->Addchild(bar);
	}

	//left Luna UI
	{
		mLunaPanal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(-6.4f, 2.0f, 10.f), Vector3::One, L"LunaPanal", this));

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				Vector3 position = Vector3(-0.11f + (0.33f * i), (0.40f - (0.35f * j)) + (0.01f * i), 0.f);


				HUD* luna = (GETSINGLE(UIFactory)->CreateHud(L"Luna" + std::to_wstring(i) + std::to_wstring(j * 10), L"LunaMaterial",
					position,
					Vector3(0.3f, 0.3f, 1.f),
					mLunaPanal, this));
				luna->Die();
				mLunaPanal->Addchild(luna);

				HUD* dottedLine = (GETSINGLE(UIFactory)->CreateHud(L"DottedLine" + std::to_wstring(i) + std::to_wstring(j * 10), L"DottedLineMaterial",
					position,
					Vector3(0.3f, 0.3f, 1.f),
					mLunaPanal, this));

				mLunaPanal->Addchild(dottedLine);
			}
		}
	}
}
