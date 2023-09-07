#include "Scene.h"
#include "InputMgr.h"
#include "Layer.h"



Scene::Scene()
	: mDeleteObj(false)
	, mType(SceneMgr::eSceneType::End)
	, mbPause(false)
	, mDeadObjects{}

{
	mLayers.resize(static_cast<UINT>(eLayerType::End));
}
Scene::~Scene()
{
}
void Scene::Initialize()
{
	for (Layer& layer : mLayers)
	{
		layer.Initialize();
	}
}

void Scene::update()
{
	for (Layer& layer : mLayers)
	{
		layer.update();
	}
}

void Scene::fixedUpdate()
{
	for (Layer& layer : mLayers)
	{
		layer.fixedUpdate();
	}
}

void Scene::render()
{
	for (Layer& layer : mLayers)
	{
		layer.render();
	}
}

void Scene::fontRender()
{
	for (Layer& layer : mLayers)
	{
		layer.fontRender();
	}
}

void Scene::destroy()
{
	for (Layer& layer : mLayers)
	{
		layer.destroy();
	}

}

void Scene::Enter()
{
	//Initialize();
}

void Scene::Exit()
{
	if (mDeleteObj)
	{
		for (Layer& layer : mLayers)
		{
			layer.DeleteObject();
		}
		mDeleteObj = false;
	}

	destroy();
}

void Scene::AddGameObject(GameObj* gameObj, eLayerType eLayer)
{
	mLayers[static_cast<UINT>(eLayer)].AddGameObject(gameObj, eLayer);
}


std::vector<GameObj*> Scene::GetDontDestroyObjects()
{
	std::vector<GameObj*> allLayerDontDestroyObjs;
	for (Layer& layer : mLayers)
	{
		std::vector<GameObj*> dontDestroyObjs = layer.GetDontDestroyObjects();

		allLayerDontDestroyObjs.insert(allLayerDontDestroyObjs.end(), dontDestroyObjs.begin(), dontDestroyObjs.end());

	}

	return allLayerDontDestroyObjs;
}

const std::vector<GameObj*>& Scene::GetGameObj(eLayerType _eLayer)
{

	return mLayers[static_cast<UINT>(_eLayer)].GetGameObjects();
}