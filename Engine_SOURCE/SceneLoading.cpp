#include "SceneLoading.h"
#include "AsyncLoad.h"
#include "Panal.h"
#include "UIFactory.h"
#include "GameThread.h"


SceneLoading::SceneLoading()
{
}

SceneLoading::~SceneLoading()
{
}

void SceneLoading::Initialize()
{
	CreateCameras();

	std::thread thread1([]()
	{
		GETSINGLE(AsyncLoad)->LoadTextures();
	});
	std::thread thread2([]()
	{
		GETSINGLE(AsyncLoad)->LoadModels();
	});
	std::thread thread3([]()
	{
			GETSINGLE(AsyncLoad)->LoadSounds();
	});
		
	thread1.detach();
	thread2.detach();
	thread3.detach();

	Panal* panal = (GETSINGLE(UIFactory)->CreatePanal(mUICamera, Vector3(0.0f, 0.0f, 0.f), Vector3(10.0f, 10.0f, 1.0f), L"LoadingScenePanal", this));
	ImageUI* filter = (GETSINGLE(UIFactory)->CreateImage(L"LoadingImg", L"loadingImgMaterial", Vector3(0.f, 0.f, 99.f), Vector3(20.f, 20.f, 1.f), panal, this));


	Scene::Initialize();
}

void SceneLoading::update()
{
	if (GETSINGLE(AsyncLoad)->IsLoadFinish())
	{
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Play);
		return;
	}

	GETSINGLE(AsyncLoad)->Update();
	Scene::update();
}

void SceneLoading::fixedUpdate()
{
	Scene::fixedUpdate();
}

void SceneLoading::render()
{
}

void SceneLoading::Enter()
{
	Scene::Enter();
}

void SceneLoading::Exit()
{
	Scene::Exit();
}
