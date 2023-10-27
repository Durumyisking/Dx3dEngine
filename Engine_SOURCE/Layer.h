#pragma once
#include "Entity.h"



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
	std::vector<GameObj*>& GetAddedGameObjects() { return mAddedObjects; }
	std::vector<GameObj*> GetDontDestroyObjects();

	void PushAddedObject(GameObj* gameObj, eLayerType type);




private:
	std::vector<GameObj*>	mGameObjs;
	std::vector<GameObj*>	mAddedObjects;

};

typedef const std::vector<GameObj*>& GameObjects;
typedef std::vector<GameObj*>::iterator GameObjectIter;
