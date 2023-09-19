#include "SceneMgr.h"

#include "Scene.h"
#include "SceneTitle.h"
#include "ScenePlay.h"
#include "Layer.h"
#include "GameObj.h"



SceneMgr::SceneMgr()
	: mScenes{}
	, mActiveScene(nullptr)
{
}

SceneMgr::~SceneMgr()
{

}

void SceneMgr::Initialize()
{
	//mScenes[static_cast<UINT>(eSceneType::Title)] = new SceneTitle;
	//mScenes[static_cast<UINT>(eSceneType::Title)]->SetType(eSceneType::Title);

	mScenes[static_cast<UINT>(eSceneType::Play)] = new ScenePlay;
	mScenes[static_cast<UINT>(eSceneType::Play)]->SetType(eSceneType::Play);

	mActiveScene = mScenes[static_cast<UINT>(eSceneType::Play)];

	for (UINT i = 0; i < static_cast<UINT>(eSceneType::End); i++)
	{
		if(mScenes[i])
			mScenes[i]->Initialize();
	}

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

void SceneMgr::DontDestroyOnLoad(GameObj* gameObj)
{
	if (nullptr == gameObj)
		return;

	gameObj->SetDestroyOff();
}
