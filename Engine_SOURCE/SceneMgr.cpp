#include "SceneMgr.h"

#include "Scene.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "ScenePlay.h"
#include "Layer.h"
#include "GameObj.h"

#include "PhysicalGameObj.h"
#include "Sphere.h"
#include "Box.h"
#include "DynamicObject.h"

#include "Monster.h"
#include "Goomba.h"
#include "Packun.h"

#include "Player.h"
#include "MarioParts.h"
#include "MarioCap.h"
#include "ModelObj.h"
#include "SkySphere.h"

#include "CityObjects.h"




SceneMgr::SceneMgr()
	: mScenes{}
	, mActiveScene(nullptr)
{
}

SceneMgr::~SceneMgr()
{
	GameObj::ClearObjectCDO();
}

void SceneMgr::Initialize()
{
	mScenes[static_cast<UINT>(eSceneType::Loading)] = new SceneLoading;
	mScenes[static_cast<UINT>(eSceneType::Loading)]->SetType(eSceneType::Loading);

	mScenes[static_cast<UINT>(eSceneType::Title)] = new SceneTitle;
	mScenes[static_cast<UINT>(eSceneType::Title)]->SetType(eSceneType::Title);

	mScenes[static_cast<UINT>(eSceneType::Play)] = new ScenePlay;
	mScenes[static_cast<UINT>(eSceneType::Play)]->SetType(eSceneType::Play);

	mActiveScene = mScenes[static_cast<UINT>(eSceneType::Loading)];
	mScenes[static_cast<UINT>(eSceneType::Test)] = new ScenePlay;
	mScenes[static_cast<UINT>(eSceneType::Test)]->SetType(eSceneType::Test);

	//for (UINT i = 0; i < static_cast<UINT>(eSceneType::End); i++)
	//{
	//	if(mScenes[i])
	//		mScenes[i]->Initialize();
	//}

	CreateCDO();

	mActiveScene->Enter();

	//for (Scene* scene : mScenes)
	//{
	//	scene->Enter();
	//}
}

void SceneMgr::Update()
{
	mActiveScene->update();
}

void SceneMgr::FixedUpdate()
{
	mActiveScene->fixedUpdate();
}

void SceneMgr::Render()
{
	mActiveScene->render();
}

void SceneMgr::FontRender()
{
	mActiveScene->fontRender();
}

void SceneMgr::Destory()
{
	mActiveScene->destroy();
}

void SceneMgr::Release()
{
	for (Scene* scene : mScenes)
	{
		if (nullptr != scene)
			delete scene;
	}
}

void SceneMgr::LoadScene(eSceneType type)
{
	if (mActiveScene->GetType() == type)
	{
		//로딩 씬으로 넘어가서 작업하기
	}

	if (mActiveScene)
		mActiveScene->Exit();

	std::vector<GameObj*> gameObjs = mActiveScene->GetDontDestroyObjects();

	mActiveScene = mScenes[static_cast<UINT>(type)];

	for (GameObj* obj : gameObjs)
	{
		mActiveScene->AddGameObject(obj, obj->GetLayerType());
	}

	if (mActiveScene)
		mActiveScene->Enter();
}

void SceneMgr::ChangeScene(eSceneType type)
{
	if (mActiveScene->GetType() == type)
	{
		//로딩 씬으로 넘어가서 작업하기
	}

	if (mActiveScene)
		mActiveScene->Exit();

	std::vector<GameObj*> gameObjs = mActiveScene->GetDontDestroyObjects();

	for (GameObj* obj : gameObjs)
	{
		obj->DeleteComponents();

		delete obj;
		obj = nullptr;
	}

	gameObjs.clear();

	mActiveScene = mScenes[static_cast<UINT>(type)];

	if (mActiveScene)
		mActiveScene->Enter();
}


void SceneMgr::LateEvent()
{
	if (mActiveScene == nullptr)
		return;

	for (GameObj* obj : mLateEvent)
	{
		enums::eLayerType type = obj->GetLayerType();
		mActiveScene->GetLayer(type).AddGameObject(obj, type);
	}

	mLateEvent.clear();
}

bool SceneMgr::SaveSceneFile(eSceneType type, const std::wstring& filePath)
{
	FILE* File = nullptr;

	_wfopen_s(&File, filePath.c_str(), L"wb");

	if (!File)
		return false;

	fwrite(&type, sizeof(eSceneType), 1, File);

	mActiveScene->Save(File);

	fclose(File);
	return true;
}

//다른 쓰레드를 써서 백그라운드 로딩 하는법 찾기
bool SceneMgr::LoadSceneFile(const std::wstring& filePath)
{
	FILE* File = nullptr;

	_wfopen_s(&File, filePath.c_str(), L"rb");

	if (!File)
		return false;

	eSceneType SceneType;
	int size = sizeof(eSceneType);
	fread(&SceneType, sizeof(eSceneType), 1, File);

	mScenes[static_cast<UINT>(SceneType)]->Load(File);

	fclose(File);

	ChangeScene(SceneType);

	//mScenes[static_cast<UINT>(SceneType)]->Initialize(); 

	return true;
}

void SceneMgr::DontDestroyOnLoad(GameObj* gameObj)
{
	if (nullptr == gameObj)
		return;

	gameObj->SetDestroyOff();
}

void SceneMgr::CreateCDO()
{
	///////// GameObject ///////////
	GameObj* GameObjCDO = new GameObj();
	GameObj::AddObjectCDO("GameObj", GameObjCDO);

	PhysicalGameObj* PhysicalGameObjCDO = new PhysicalGameObj();
	GameObj::AddObjectCDO("PhysicalGameObj", PhysicalGameObjCDO);

	Sphere* SphereCDO = new Sphere();
	GameObj::AddObjectCDO("Sphere", SphereCDO);

	Box* BoxCDO = new Box();
	GameObj::AddObjectCDO("Box", BoxCDO);

	DynamicObject* DynamicObjectCDO = new DynamicObject();
	GameObj::AddObjectCDO("DynamicObject", DynamicObjectCDO);

	Player* PlayerCDO = new Player();
	GameObj::AddObjectCDO("Player", PlayerCDO);

	Monster* MonsterCDO = new Monster();
	GameObj::AddObjectCDO("Monster", MonsterCDO);

	Goomba* GoombaCDO = new Goomba();
	GameObj::AddObjectCDO("Goomba", GoombaCDO);

	Packun* PackunCDO = new Packun();
	GameObj::AddObjectCDO("Packun", PackunCDO);

	MarioParts* MarioPartsCDO = new MarioParts();
	GameObj::AddObjectCDO("MarioParts", MarioPartsCDO);

	MarioCap* MarioCapCDO = new MarioCap();
	GameObj::AddObjectCDO("MarioCap", MarioCapCDO);

	ModelObj* ModelObjCDO = new ModelObj();
	GameObj::AddObjectCDO("ModelObj", ModelObjCDO);

	HomeBuildingEight* buildingEightCDO = new HomeBuildingEight();
	GameObj::AddObjectCDO("HomeBuildingEight", buildingEightCDO);

	//Building* buildingCDO = new Building();
	//GameObj::AddObjectCDO("Building", buildingCDO);

	//HomeBuildingFour* buildingFourCDO = new HomeBuildingFour();
	//GameObj::AddObjectCDO("HomeBuildingFour", buildingFourCDO);

	//HomeBuildingFive* buildingFiveCDO = new HomeBuildingFive();
	//GameObj::AddObjectCDO("HomeBuildingFive", buildingFiveCDO);
	//
	//HomeBuildingSeven* buildingSevenCDO = new HomeBuildingSeven();
	//GameObj::AddObjectCDO("HomeBuildingSeven", buildingSevenCDO);

	//Car* carCDO = new Car();
	//GameObj::AddObjectCDO("Car", carCDO); //out of range

	//CoinObject* coinCDO = new CoinObject();
	//GameObj::AddObjectCDO("CoinObject", coinCDO);

	//CityWorldBush* bushCDO = new CityWorldBush();
	//GameObj::AddObjectCDO("CityWorldBush", bushCDO);

	//CityWorldBushA* bushACDO = new CityWorldBushA();
	//GameObj::AddObjectCDO("CityWorldBushA", bushACDO);

	//CityWorldChairA* chairACDO = new CityWorldChairA();
	//GameObj::AddObjectCDO("CityWorldChairA", chairACDO);

	//CityWorldChairB* chairBCDO = new CityWorldChairB();
	//GameObj::AddObjectCDO("CityWorldChairB", chairBCDO);

	//BenchA* benchACDO = new BenchA();
	//GameObj::AddObjectCDO("BenchA", benchACDO);

	//HomeBuilding_09* building_9CDO = new HomeBuilding_09();
	//GameObj::AddObjectCDO("HomeBuilding_09", building_9CDO);

	//HomeBuilding_10* building_10CDO = new HomeBuilding_10();
	//GameObj::AddObjectCDO("HomeBuilding_10", building_10CDO);

	//HomeBuilding_11* building_11CDO = new HomeBuilding_11();
	//GameObj::AddObjectCDO("HomeBuilding_11", building_11CDO);

	//HomeBuilding_12* building_12CDO = new HomeBuilding_12();
	//GameObj::AddObjectCDO("HomeBuilding_12", building_12CDO);

	//HomeBuilding_13* building_13CDO = new HomeBuilding_13();
	//GameObj::AddObjectCDO("HomeBuilding_13", building_13CDO);

	//SkySphere* SkySphere = new SkySphere();
	//GameObj::AddObjectCDO("SkySphere", SkySphere);
}