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

#include "Building.h"
#include "CreateObject.h"


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

	mScenes[static_cast<UINT>(SceneType)]->Initialize();

	LoadScene(SceneType);

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

	Building* buildingCDO = new Building();
	GameObj::AddObjectCDO("Building", buildingCDO);

	CreateObject* building000CDO = new CreateObject(L"CityWorldHomeBuilding000", L"HomeBuilding000_0Material", Vector3::One); //오류 발생함
	GameObj::AddObjectCDO("CityWorldHomeBuilding000", building000CDO);

	CreateObject* building001CDO = new CreateObject(L"CityWorldHomeBuilding001", L"HomeBuilding001_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding001", building001CDO);

	CreateObject* building002CDO = new CreateObject(L"CityWorldHomeBuilding002", L"HomeBuilding002_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding002", building002CDO);

	CreateObject* building003CDO = new CreateObject(L"CityWorldHomeBuilding003", L"HomeBuilding003_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding003", building003CDO);

	CreateObject* building004CDO = new CreateObject(L"CityWorldHomeBuilding004", L"HomeBuilding004_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding004", building004CDO);

	CreateObject* building005CDO = new CreateObject(L"CityWorldHomeBuilding005", L"HomeBuilding005_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding005", building005CDO);

	CreateObject* building007CDO = new CreateObject(L"CityWorldHomeBuilding007", L"HomeBuilding007_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding007", building007CDO);

	CreateObject* building008CDO = new CreateObject(L"CityWorldHomeBuilding008", L"HomeBuilding008_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding008", building008CDO);

	CreateObject* building009CDO = new CreateObject(L"CityWorldHomeBuilding009", L"HomeBuilding009_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding009", building009CDO);

	CreateObject* building010CDO = new CreateObject(L"CityWorldHomeBuilding010", L"HomeBuilding0010_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding010", building010CDO);

	CreateObject* building011CDO = new CreateObject(L"CityWorldHomeBuilding011", L"HomeBuilding0011_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding011", building011CDO);

	CreateObject* building012CDO = new CreateObject(L"CityWorldHomeBuilding012", L"HomeBuilding0012_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding012", building012CDO);

	CreateObject* building013CDO = new CreateObject(L"CityWorldHomeBuilding013", L"HomeBuilding0013_0Material", Vector3::One);
	GameObj::AddObjectCDO("CityWorldHomeBuilding013", building013CDO);


	//SkySphere* SkySphere = new SkySphere();
	//GameObj::AddObjectCDO("SkySphere", SkySphere);
}