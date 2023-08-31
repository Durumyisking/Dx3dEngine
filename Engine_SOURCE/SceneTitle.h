#pragma once
#include "Scene.h"


class Panal;
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


private:
	GameObj*	mCamera;
	GameObj*    mUICamera;

	Panal* panal;

};

