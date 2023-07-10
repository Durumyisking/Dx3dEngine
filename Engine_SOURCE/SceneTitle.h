#pragma once
#include "Scene.h"

namespace dru
{

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

	};

}


