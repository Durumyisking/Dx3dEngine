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

#include "UIManager.h"
#include "Panal.h"
#include "Button.h"
#include "ImageUI.h"
#include "UIFactory.h"
#include "Animator.h"
#include "LifeUI.h"

#include "CoinUIScript.h"
#include "LifeUIScript.h"
#include "CompassUIScript.h"
#include "PowerMoonScript.h"
#include "CompassNeedleScript.h"
#include "DieUIEffectScript.h"
#include "DieCircleUIScript.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Building.h"

#include "BlockBrick.h"

#include "CityGround.h"
#include "CoinObject.h"
#include "Car.h"
#include "CityMapNaviObject.h"
#include "CityWorldBush.h"
#include "CityWorldBushA.h"
#include "CityWorldChairA.h"
#include "CityWorldChairB.h"
#include "CityWorldFlag.h"
#include "BenchA.h"
#include "CheckpointFlag.h"
#include "HomeBuildingFour.h"
#include "HomeBuildingFive.h"
#include "HomeBuildingSeven.h"
#include "HomeBuildingEight.h"

#include "CreateObject.h"

#include "Goomba.h"
#include "Packun.h"

#include "PostProcess.h"


#include "InstancingContainer.h"


#include "NavigationMgr.h"
#include "PathMgr.h"


#include "ModelObj.h"

ScenePlay::ScenePlay()
	: mCoinPanal(nullptr)
	, mCityCoinPanal(nullptr)
	, mLifePanal(nullptr)
	, mLunaPanal(nullptr)
	, mCompassPanal(nullptr)
	, mDieUIPanal(nullptr)
	, mPlayer(nullptr)
{
	SetName(L"ScenePlay");
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::Save(FILE* File)
{
}

void ScenePlay::Load(FILE* File)
{
}

void ScenePlay::Initialize()
{
	CreateCameras();

	//TestScene �ε� �׽�Ʈ �ε�ÿ� �ݺ��ؼ� ���� ���� ����
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
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Player, eLayerType::Objects);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Objects, eLayerType::Monster);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Monster, eLayerType::Platforms);
	GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Monster, eLayerType::Cap);
	//Convex and Triangle Mesh TEST
	
	////TriangleMesh Test
	//{
	//	ModelObj* obj = object::Instantiate<ModelObj>(eLayerType::Platforms, this);
	//	obj->SetPos(Vector3(0.f, 5.f, 0.f));
	//	obj->SetScale(Vector3(0.01f, 0.01f, 0.01f));
	//	obj->SetName(L"CityWorldHomeGroundCollider");

	//	// SetModel
	//	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorld_RoadCollider");
	//	if (model)
	//	{
	//		obj->SetModel(model->GetName());
	//		obj->SetPhysical(eGeometryType::TriangleMesh, Vector3(1.f, 1.f, 1.f));
	//	}
	//}

	{
		MarioCap* mariocap = object::Instantiate<MarioCap>(eLayerType::Cap, this);
		mPlayer = object::Instantiate<Player>(eLayerType::Player, this);
		mPlayer->SetMarioCap(mariocap);
		
		mCamera->GetComponent<Camera>()->SetTarget(mPlayer);
	}
	//{
	//	Goomba* goomba = object::Instantiate<Goomba>(eLayerType::Monster, this);
	//	goomba->SetPos(Vector3(35.f, 10.f, 30.f));
	//}	
	//{
	//	Goomba* goomba = object::Instantiate<Goomba>(eLayerType::Monster, this);
	//	goomba->SetPos(Vector3(25.f, 10.f, -10.f));	
	//}
	//{
	//	Goomba* goomba = object::Instantiate<Goomba>(eLayerType::Monster, this);
	//	goomba->SetPos(Vector3(-25.f, 10.f, -10.f));
	//}


	{
		PostProcess* mPostProcess_Replay = object::Instantiate<PostProcess>(eLayerType::PostProcess, L"PostProcess_LensFlare");
		mPostProcess_Replay->SetMaterial(L"BasicPostProcessMaterial");
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
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"DeferredMaterial_NT");
		plane->GetMeshRenderer()->GetMaterial()->SetMetallic(0.99f);
		plane->GetMeshRenderer()->GetMaterial()->SetRoughness(0.01f);
		plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		rigid->RemoveGravity();

		plane->AddComponent<PhysXCollider>(eComponentType::Collider);

	}


	{
		Building* block = object::Instantiate<Building>(eLayerType::Objects, this, L"Building");
		block->SetPos(Vector3(40.f, -0.5f, 0.f));

		//CityGround* ground = object::Instantiate<CityGround>(eLayerType::Objects, this, L"CityGround");
		//ground->SetPos(Vector3(100.f, -10.f, 0.f));

		//CoinObject* coin = object::Instantiate<CoinObject>(eLayerType::Objects, this, L"CoinObject");
		//coin->SetPos(Vector3(0.f, 1.f, 0.f));

		//Car* car = object::Instantiate<Car>(eLayerType::Objects, this, L"car");
		//car->SetPos(Vector3(40.f, 10.f, 0.f));

		//CityMapNaviObject* nai = object::Instantiate<CityMapNaviObject>(eLayerType::Objects, this, L"navi");
		//nai->SetPos(Vector3(0.f, 10.f, 0.f));

		//CityWorldBush* bush = object::Instantiate<CityWorldBush>(eLayerType::Objects, this, L"bush");
		//bush->SetPos(Vector3(0.f, 10.f, 0.f));

		//CityWorldBushA* bush = object::Instantiate<CityWorldBushA>(eLayerType::Objects, this, L"bushA");
		//bush->SetPos(Vector3(0.f, 10.f, 0.f));

		//CityWorldChairA* chair = object::Instantiate<CityWorldChairA>(eLayerType::Objects, this, L"chairA");
		//chair->SetPos(Vector3(0.f, 1.f, 0.f));

		//CityWorldChairB* chair = object::Instantiate<CityWorldChairB>(eLayerType::Objects, this, L"chairB");
		//chair->SetPos(Vector3(0.f, 1.f, 0.f));

		//CityWorldFlag* chair = object::Instantiate<CityWorldFlag>(eLayerType::Objects, this, L"CityWorldFlag");
		//chair->SetPos(Vector3(0.f, 1.f, 0.f));

		//BenchA* bench = object::Instantiate<BenchA>(eLayerType::Objects, this, L"BenchA");
		//bench->SetPos(Vector3(0.f, 1.f, 0.f));

		//CheckpointFlag* checkPointFlag = object::Instantiate<CheckpointFlag>(eLayerType::Objects, this, L"CheckpointFlag");
		//checkPointFlag->SetPos(Vector3(0.f, 1.f, 0.f));

		//HomeBuildingFour* homeBuildingFour = object::Instantiate<HomeBuildingFour>(eLayerType::Objects, this, L"homeBuildingFour");
		//homeBuildingFour->SetPos(Vector3(0.f, 1.f, 0.f));

		//HomeBuildingFive* homeBuildingFive = object::Instantiate<HomeBuildingFive>(eLayerType::Objects, this, L"homeBuildingFive");
		//homeBuildingFive->SetPos(Vector3(0.f, 1.f, 0.f));

		//HomeBuildingSeven* homeBuildingSeven = object::Instantiate<HomeBuildingSeven>(eLayerType::Objects, this, L"homeBuildingSeven");
		//homeBuildingSeven->SetPos(Vector3(0.f, 1.f, 0.f));

		//HomeBuildingEight* homeBuildingEight = object::Instantiate<HomeBuildingEight>(eLayerType::Objects, this, L"homeBuildingEight");
		//homeBuildingEight->SetPos(Vector3(0.f, 1.f, 0.f));

		//std::vector<std::wstring> array = { L"GlassBuildingWallMaterial" ,L"RoofConcrete00Material" ,L"RoofConcrete01Material" ,L"WallGlassPaintedSteelMaterial" };
		//Building* block = object::Instantiate<Building>(eLayerType::Objects, this, L"Building", L"CityWorldHomeBuilding002",array);
		//block->SetPos(Vector3(40.f, -10.f, 0.f));

		//std::vector<std::wstring> array = { L"HomeBuilding009_0Material" ,L"HomeBuilding009_1Material" ,L"HomeBuilding009_2Material" ,L"HomeBuilding009_2Material",L"HomeBuilding009_4Material"
		//	,L"HomeBuilding009_5Material",  L"HomeBuilding009_6Material" ,L"HomeBuilding009_7Material" ,L"HomeBuilding009_8Material" };
		//CreateObject* object = object::Instantiate<CreateObject>(eLayerType::Objects, this, L"Building", L"CityWorldHomeBuilding009", array);
		//object->SetPos(Vector3(40.f, -10.f, 0.f));

	}
	InstancingContainer* blockContainer = object::Instantiate<InstancingContainer>(eLayerType::ObjectsContainer, this, L"BlockBrickContainer");
	for (size_t i = 0; i < 20; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			for (size_t k = 1; k < 10; k++)
			{
				if (j > 4 && k > 1)
					continue;

				BlockBrick* block = object::Instantiate<BlockBrick>(eLayerType::Objects, this, L"BlockBrick");
				block->SetPos(Vector3(1.f * i, 1.f * k, 1.f * j));
				blockContainer->PushObject(block);
			}
		}
	}
	blockContainer->ResizeObjectWorldMatrix();

	{
		SoloNaviMesh* naviMesh = GETSINGLE(NavigationMgr)->CreateNavigationMesh();

		//현재 .obj 파일만 로딩 가능 블랜더에서 .obj 로 내보내기 해서 사용하면 됩니다
		if (!GETSINGLE(NavigationMgr)->SettingMesh(naviMesh, GETSINGLE(PathMgr)->FindPath(OBJ_SAVE_PATH) + L"CityWorld_HomeStage_GroundCollider.Obj"))
			int debug = 0;

		if (!naviMesh->Build())
			int debug = 0;

		//오브젝트에 std::<Vector3>mPath 추가 path에 이동경로가 추가되니 vector내의 위치를 사용해서 이동하면 됩니다
		//위치가 내비메쉬 밖이면 계산이 안됩니다
		if(!GETSINGLE(NavigationMgr)->FindPath(mPlayer, Vector3(10.f, 1.f, 30.f)))
			int debug = 0;
	}


	CreatePlayerUI();

	Scene::Initialize();
}


void ScenePlay::update()
{
	if (KEY_TAP(F_9))
	{
		//mCoinPanal->GetScript<CoinUIScript>()->GetCoin();
		//mCityCoinPanal->GetScript<CoinUIScript>()->GetCoin();
		(GETSINGLE(UIManager)->PlayerDie());
		//mCoinPanal->GetScript<CoinUIScript>()->ActionToPlayerDied();
	}

	if (KEY_TAP(F_2))
	{
		mCoinPanal->GetScript<CoinUIScript>()->GetCoin();
	}

	if (KEY_TAP(F_3))
	{
		mCoinPanal->GetScript<CoinUIScript>()->Reset();
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

	mCamera->SetPos(Vector3(0.f, 25.f, -25.f));
	mCamera->GetComponent<Transform>()->SetRotationX(45.f);
	//mCamera->GetComponent<Camera>()->SetTarget(mPlayer);
}

void ScenePlay::Exit()
{
	Scene::Exit();
}

void ScenePlay::CreatePlayerUI()
{
	//Life UI
	{
		mLifePanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(700.f, 370.0f, 10.f), Vector3(100.0f, 100.0f, 1.0f), L"LifePanal", this, eUIType::HP));
		mLifePanal->AddComponent<LifeUIScript>(eComponentType::Script);
		//LifeGauge Vector3(7.f, 3.6f, 0.f)
		ImageUI* gauge = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"LifeGauge", L"LifeGauge_3Material", eUIType::None, Vector3(0.f, 0.f, 0.f), Vector3::One, mLifePanal, this));
		gauge->SetUIActive();

		//LifeText Vector3(7.f, 3.55f, -0.1f)
		ImageUI* lifeText = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"LifeText", L"LifeTextMaterial", eUIType::HPText, Vector3(0.f,  -0.05f, -0.1f), Vector3(0.4f, 0.4f, 1.f), mLifePanal, this));
		lifeText->SetColor(Vector4(0.1f, 0.1f, 0.1f, 1.0f),true);
		lifeText->SetUIActive();

		//Lifeheart Vector3(7.f, 3.55f, 0.f)
		ImageUI* lifeheart = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"LifeHeart", L"LifeheartMaterial", eUIType::None, Vector3(0.f, -0.05f, 0.f), Vector3(0.6f, 0.6f, 1.0f), mLifePanal, this));
		lifeheart->SetUIActive();


		mLifePanal->Addchild(gauge);
		mLifePanal->Addchild(lifeheart);
		mLifePanal->Addchild(lifeText);
	}

	//Left Coin UI
	{
		mCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.f, 0.f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CoinPanal", this, eUIType::Coin));
		mCoinPanal->AddComponent<CoinUIScript>(eComponentType::Script);



		ImageUI* coin = (GETSINGLE(UIFactory)->CreateImage(L"Coin", L"CoinMaterial", Vector3(-7.f, 3.5f, 0.f), Vector3(1.f, 1.f, 1.f), mCoinPanal, this));
		coin->SetUIActive();
		ImageUI* bar = (GETSINGLE(UIFactory)->CreateImage(L"Bar", L"BarMaterial", Vector3(-5.4f, 2.9f, 0.f), Vector3(4.2f, 1.4f, 1.f), mCoinPanal, this));
		mCoinPanal->Addchild(coin);

		for (size_t i = 0; i < 3; i++)
		{
			ImageUI* image = (GETSINGLE(UIFactory)->CreateImage(L"CoinText", L"CoinTextMaterial_" + std::to_wstring(i), Vector3(-6.35f + (0.34f * i), 3.3f, 0.f), Vector3(0.5f, 0.5f, 1.0f), mCoinPanal, this));
			image->SetUIActive();

			mCoinPanal->Addchild(image);
		}

		mCoinPanal->Addchild(bar);

		mCityCoinPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CityCoinPanal", this, eUIType::CityCoin));
		mCityCoinPanal->AddComponent<CoinUIScript>(eComponentType::Script);
		ImageUI* cityCoin = (GETSINGLE(UIFactory)->CreateImage(L"CityCoin", L"CityCoinMaterial", Vector3(-5.f, 3.6f, 0.f), Vector3(1.f, 1.f, 1.f), mCityCoinPanal, this));
		cityCoin->SetUIActive();


		mCityCoinPanal->Addchild(cityCoin);
		for (size_t i = 0; i < 3; i++)
		{
			ImageUI* image = (GETSINGLE(UIFactory)->CreateImage(L"CoinText", L"CityCoinTextMaterial_" + std::to_wstring(i), Vector3(-4.35f + (0.34f * i), 3.3f, 0.f), Vector3(0.5f, 0.5f, 1.0f), mCityCoinPanal, this));
			image->SetUIActive();

			mCityCoinPanal->Addchild(image);
		}

	}

	//left Luna UI
	{
		mLunaPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"LunaPanal", this, eUIType::Luna));
		mLunaPanal->AddComponent<PowerMoonScript>(eComponentType::Script);

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				Vector3 position = Vector3(-7.f + (0.33f * i), (2.5f - (0.35f * j)) + (0.01f * i), 0.f);

				ImageUI* luna = (GETSINGLE(UIFactory)->CreateImage(L"Luna" + std::to_wstring(i) + std::to_wstring(j * 10), L"LunaMaterial",
					position,
					Vector3(0.3f, 0.3f, 1.f),
					mLunaPanal, this));

				luna->SetUIOn(false);
				luna->InActivate();
				mLunaPanal->Addchild(luna);
			}
		}

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				Vector3 position = Vector3(-7.f + (0.33f * i), (2.5f - (0.35f * j)) + (0.01f * i), 0.f);

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
		mCompassPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"CompassPanal", this, eUIType::Compass));

		ImageUI* compassBar = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"CompassBar", L"CompassBarMaterial", eUIType::None, Vector3(7.f, 2.5f, 0.f), Vector3::One, mCompassPanal, this));
		ImageUI* compassNeedle = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"CompassNeedle", L"CompassNeedleMaterial", eUIType::None, Vector3(7.f, 2.5f, 0.f), Vector3(2.0f, 2.0f, 1.0f), mCompassPanal, this));
		ImageUI* compass = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"Compass", L"CompassMaterial", eUIType::None, Vector3(7.f, 2.5f, 0.f), Vector3::One, mCompassPanal, this));
		compassBar->AddComponent<CompassUIScript>(eComponentType::Script);
		compassNeedle->AddComponent<CompassNeedleScript>(eComponentType::Script);
		compassNeedle->GetScript<CompassNeedleScript>()->SetPlayer(mPlayer);

		mCompassPanal->Addchild(compassBar);
		mCompassPanal->Addchild(compassNeedle);
		mCompassPanal->Addchild(compass);
	}

	// DieUI
	{
		mDieUIPanal = (GETSINGLE(UIFactory)->CreatePanal(renderer::UICamera->GetOwner(), Vector3(0.0f, 0.0f, 0.f), Vector3(100.0f, 100.0f, 1.0f), L"DiePanal", this, eUIType::DiePanal));
		mDieUIPanal->AddComponent<DieUIEffectScript>(eComponentType::Script);

		ImageUI* dieCircle = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"DieCircle", L"DieCircleMaterial", eUIType::None, Vector3(0.f, 0.f, 0.f), Vector3(20.0f,20.0f,1.0f), mDieUIPanal, this));
		dieCircle->AddComponent<DieCircleUIScript>(eComponentType::Script);

		ImageUI* dieTexture = (GETSINGLE(UIFactory)->CreateUI<ImageUI>(L"DieTexture", L"DieTextureMaterial", eUIType::None, Vector3(0.f, 0.f, 0.f), Vector3(20.0f,10.0f,1.0f), mDieUIPanal, this));

		dieCircle->SetColor(Vector4(1.0f, 1.0f, 1.f, 0.1f), true);
		mDieUIPanal->Addchild(dieCircle);
		mDieUIPanal->Addchild(dieTexture);
		mDieUIPanal->InActivate();
	}
}