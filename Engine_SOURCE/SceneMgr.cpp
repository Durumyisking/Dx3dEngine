#include "SceneMgr.h"

#include "Scene.h"
#include "SceneTitle.h"
#include "Layer.h"
#include "GameObj.h"

namespace dru
{
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
		mScenes[static_cast<UINT>(eSceneType::Title)] = new SceneTitle;
		mScenes[static_cast<UINT>(eSceneType::Title)]->SetType(eSceneType::Title);

		for (UINT i = 0; i < static_cast<UINT>(eSceneType::End); i++)
		{
			mScenes[i]->Initialize();
		}

		mActiveScene = mScenes[static_cast<UINT>(eSceneType::Title)];
		mActiveScene->Enter();


	}

	void SceneMgr::update()
	{
		mActiveScene->update();
	}

	void SceneMgr::fixedUpdate()
	{
		mActiveScene->fixedUpdate();
	}

	void SceneMgr::render()
	{
		mActiveScene->render();
	}

	void SceneMgr::fontRender()
	{
		mActiveScene->fontRender();
	}

	void SceneMgr::destory()
	{
		mActiveScene->destroy();
	}

	void SceneMgr::release()
	{
		for (Scene* scene : mScenes)
		{
			if (nullptr != scene)
				delete scene;
		}
	}

	void SceneMgr::LoadScene(eSceneType _Type)
	{
		if (mActiveScene)
			mActiveScene->Exit();

		std::vector<GameObj*> gameObjs = mActiveScene->GetDontDestroyObjects();

		mActiveScene = mScenes[static_cast<UINT>(_Type)];

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

	void SceneMgr::DontDestroyOnLoad(GameObj* _GameObj)
	{
		if (nullptr == _GameObj)
			return;

		_GameObj->DontDestroy();
	}

}