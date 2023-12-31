#pragma once
#include "Scene.h"


class Panal;
class HUD;
class Player;
class ScenePlay :
	public Scene
{
public:
	ScenePlay();
	virtual ~ScenePlay();

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	virtual void Initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void render();

	virtual void Enter();
	virtual void Exit();

	void SetType(SceneMgr::eSceneType eType) { mType = eType; }
	SceneMgr::eSceneType GetType() const { return mType; }


	void CreatePlayerUI();
	Player* GetPlayer() { return mPlayer; }
private:
	Panal* mCoinPanal;
	Panal* mCityCoinPanal;
	Panal* mLifePanal;
	Panal* mLunaPanal;
	Panal* mCompassPanal;
	Panal* mDieUIPanal;

	Player* mPlayer;
};