#pragma once
#include "Scene.h"

class SceneLoading :
    public Scene
{
public:
	SceneLoading();
	virtual ~SceneLoading();

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	virtual void Initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void render();

	virtual void Enter();
	virtual void Exit();

	
};

