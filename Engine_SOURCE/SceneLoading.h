#pragma once
#include "Scene.h"

class SceneLoading :
    public Scene
{
public:
	SceneLoading();
	virtual ~SceneLoading();

	virtual void Initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void render();

	virtual void Enter();
	virtual void Exit();

	
};

