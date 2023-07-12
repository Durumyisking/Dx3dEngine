#pragma once
#include "Entity.h"

namespace dru
{
	class GameObj;
	class Layer : public DruEntity
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();
		virtual void fontRender();
		virtual void destroy();
		void DeleteObject ();


		void AddGameObject(GameObj* gameObj, enums::eLayerType type);
		const std::vector<GameObj*>& GetGameObjects() { return mGameObjs; }
		std::vector<GameObj*> GetDontDestroyObjects();


	private:
		std::vector<GameObj*>	mGameObjs;

	};

	typedef const std::vector<GameObj*>& GameObjects;
	typedef std::vector<GameObj*>::iterator GameObjectIter;
}
