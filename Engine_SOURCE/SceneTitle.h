#pragma once
#include "Scene.h"


class Panal;
class HUD;
class SceneTitle :
    public Scene
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void render();

	virtual void Enter();
	virtual void Exit();

	void CreateMainMenu();
private:
	GameObj*	mCamera;
	GameObj*    mUICamera;

	Panal* MainMenuPanal;

};

