#pragma once
#include "Scene.h"
#include "Player.h"


class Panal;
class HUD;
class ScenePlay :
	public Scene
{
public:
	ScenePlay();
	virtual ~ScenePlay();

	virtual void Initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void render();

	virtual void Enter();
	virtual void Exit();

	void SetType(SceneMgr::eSceneType eType) { mType = eType; }
	SceneMgr::eSceneType GetType() const { return mType; }


	void CreatePlayerUI();
private:
	Panal* mCoinPanal;
	Panal* mCityCoinPanal;
	Panal* mLifePanal;
	Panal* mLunaPanal;
	Panal* mCompassPanal;

	Player* mPlayer;
};