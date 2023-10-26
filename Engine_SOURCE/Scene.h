#pragma once
#include "Entity.h"
#include "SceneMgr.h"
#include "Layer.h"
#include "PhysXCollisionMgr.h"



using namespace enums;
class Layer;

class Scene : public DruEntity // 이름과 고유 id를 가진 class
{
public:
	Scene();
	virtual ~Scene();

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	virtual void Initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void render();
	virtual void fontRender();
	virtual void destroy();

	virtual void Enter();
	virtual void Exit();

	void SaveLayerObjects(FILE* File, eLayerType type);
	void LoadLayerObjects(FILE* File, eLayerType type);

	void AddGameObject(GameObj* gameObj, const eLayerType eLayer);

	void SetType(SceneMgr::eSceneType eType) { mType = eType; }
	SceneMgr::eSceneType GetType() const { return mType; }

	Layer& GetLayer(eLayerType type)  { return mLayers[static_cast<UINT>(type)]; }

	std::vector<GameObj*> GetDontDestroyObjects();

	const std::vector<GameObj*>& GetGameObjects(eLayerType eLayer);

	GameObj* GetPlayer();
	GameObj* FindSceneGameObject(const std::wstring& name);

protected:
	void CreateCameras();

protected :
	std::vector<Layer> mLayers;
	SceneMgr::eSceneType mType;

	std::vector<GameObj*> mDeadObjects;

	GameObj* mUICamera;
	GameObj* mCamera;

	bool mDeleteObj;

	bool mbInitialized;


public:
	bool mbPause;

};

