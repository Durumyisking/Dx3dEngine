#pragma once
#include "Entity.h"
#include "SceneMgr.h"
#include "Layer.h"

namespace dru
{
	using namespace enums;
	class Layer;

	class Scene : public DruEntity // �̸��� ���� id�� ���� class
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();
		virtual void fontRender();
		virtual void destroy();

		virtual void Enter();
		virtual void Exit();


		void AddGameObject(GameObj* _GameObj, const eLayerType _eLayer);

		void SetType(SceneMgr::eSceneType _eType) { mType = _eType; }
		SceneMgr::eSceneType GetType() const { return mType; }

		Layer& GetLayer(eLayerType _Type)  { return mLayers[static_cast<UINT>(_Type)]; }

		std::vector<GameObj*> GetDontDestroyObjects();

		const std::vector<GameObj*>& GetGameObj(eLayerType _eLayer);

	protected :
		std::vector<Layer> mLayers;
		SceneMgr::eSceneType mType;

		std::vector<GameObj*> mDeadObjects;

		bool mDeleteObj;

	public:
		bool mbPause;

	};

}

