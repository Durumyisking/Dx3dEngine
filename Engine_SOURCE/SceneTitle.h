#pragma once
#include "Scene.h"

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
<<<<<<< HEAD
	GameObj*    mUICamera;
=======
	GameObj*	mRootGameobj;
	std::vector<GameObj*>	mRoot;
>>>>>>> c22fd52be324c63ef46a67bcee5e505d422e9998

};

