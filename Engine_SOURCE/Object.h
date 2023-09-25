#pragma once
#include "Scene.h"
#include "Layer.h"
#include "Transform.h"
#include "GameObj.h"
#include "PhysicalGameObj.h"

namespace object
{
	template <typename T>
	static T* LateInstantiate(enums::eLayerType layerType)
	{
		T* gameObj = new T();
		GETSINGLE(SceneMgr)->AddEvent(gameObj);
		gameObj->SetLayerType(layerType);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType, Scene* scene)
	{
		T* gameObj = new T();
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType, Scene* scene, const std::wstring& key, eUIType type)
	{
		T* gameObj = new T(key, type);
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType,eUIType uiType ,Scene* scene)
	{
		T* gameObj = new T(uiType);
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType, Scene* scene, std::wstring name)
	{
		T* gameObj = new T();
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		gameObj->SetName(name);

		return gameObj;
	}


	template <typename T>
	static T* Instantiate(enums::eLayerType layerType, std::wstring name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		gameObj->SetName(name);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate_pooling(enums::eLayerType layerType, std::wstring name)
	{
		T* gameObj = new T();

		gameObj->SetName(name);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType, GameObj* parent, std::wstring name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);
		gameObj->SetName(name);
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetParent(parent);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType, Vector3 pos, Vector3 rotation, std::wstring name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer();
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		gameObj->SetName(name);
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(pos);
		tr->SetRotation(rotation);
		

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType layerType, Vector3 pos, Vector3 rotation, GameObj* parent, std::wstring name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer();
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);
		gameObj->SetName(name);
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(pos);
		tr->SetRotation(rotation);

		tr->SetParent(parent);
		

		return gameObj;
	}

	//이미 존재하는 오브젝트를 씬에 삽입
	template <typename T>
	static T* AddToScene(T* gameObj, enums::eLayerType layerType, Scene* scene)
	{
		Layer& layer = scene->GetLayer(layerType);
		layer.AddGameObject(gameObj, layerType);
		layer.PushAddedObject(gameObj);

		return gameObj;
	}

	static void DontDestroyOnLoad(GameObj* gameobj)
	{
		if (gameobj == nullptr)
			return;

		gameobj->SetDestroyOff();
	}
}