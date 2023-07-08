#pragma once
#include "Scene.h"
#include "Layer.h"
#include "Transform.h"
#include "GameObj.h"

namespace dru::object
{
	template <typename T>
	static T* Instantiate(enums::eLayerType _LayerType)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer(_LayerType);
		layer.AddGameObject(gameObj, _LayerType);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _LayerType, Scene* _Scene)
	{
		T* gameObj = new T();
		Layer& layer = _Scene->GetLayer(_LayerType);
		layer.AddGameObject(gameObj, _LayerType);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _LayerType, Scene* _Scene, std::wstring _Name)
	{
		T* gameObj = new T();
		Layer& layer = _Scene->GetLayer(_LayerType);
		layer.AddGameObject(gameObj, _LayerType);

		gameObj->SetName(_Name);

		return gameObj;
	}


	template <typename T>
	static T* Instantiate(enums::eLayerType _LayerType, std::wstring _Name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer(_LayerType);
		layer.AddGameObject(gameObj, _LayerType);

		gameObj->SetName(_Name);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate_pooling(enums::eLayerType _LayerType, std::wstring _Name)
	{
		T* gameObj = new T();

		gameObj->SetName(_Name);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _LayerType, GameObj* _Parent, std::wstring _Name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer(_LayerType);
		layer.AddGameObject(gameObj, _LayerType);
		gameObj->SetName(_Name);
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetParent(_Parent);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _LayerType, Vector3 _Pos, Vector3 _Rotation, std::wstring _Name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer();
		layer.AddGameObject(gameObj, _LayerType);
		gameObj->SetName(_Name);
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(_Pos);
		tr->SetRotation(_Rotation);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _LayerType, Vector3 _Pos, Vector3 _Rotation, GameObj* _Parent, std::wstring _Name)
	{
		T* gameObj = new T();
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
		Layer& layer = scene->GetLayer();
		layer.AddGameObject(gameObj, _LayerType);
		gameObj->SetName(_Name);
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(_Pos);
		tr->SetRotation(_Rotation);

		tr->SetParent(_Parent);

		return gameObj;
	}



	static void DontDestroyOnLoad(GameObj* _Gameobj)
	{
		if (_Gameobj == nullptr)
			return;

		_Gameobj->DontDestroy();
	}
}