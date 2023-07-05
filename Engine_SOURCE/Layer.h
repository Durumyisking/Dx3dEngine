#pragma once
#include "GameObj.h"

namespace dru
{
	class Layer : public Entity
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


		void AddGameObject(GameObj* _GameObj, eLayerType _Type);
		const std::vector<GameObj*>& GetGameObjects() { return mGameObjs; }
		std::vector<GameObj*> GetDontDestroyObjects();


	private:
		std::vector<GameObj*>	mGameObjs;

	};

	typedef const std::vector<GameObj*>& GameObjects;
	typedef std::vector<GameObj*>::iterator GameObjectIter;
}
