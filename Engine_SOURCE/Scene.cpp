#include "Scene.h"
#include "InputMgr.h"
#include "Layer.h"
#include "Renderer.h"
#include "Object.h"
#include "CameraScript.h"
#include "Player.h"
#include "TimerMgr.h"
Scene::Scene()
	: mDeleteObj(true)
	, mType(SceneMgr::eSceneType::End)
	, mbPause(false)
	, mDeadObjects{}
	, mUICamera(nullptr)
	, mCamera(nullptr)
	, mbInitialized(false)

{
	mLayers.resize(static_cast<UINT>(eLayerType::End));
}
Scene::~Scene()
{
}
void Scene::Initialize()
{
	for (Layer& layer : mLayers)
	{
		layer.Initialize();
	}
}

void Scene::update()
{
	for (Layer& layer : mLayers)
	{
		layer.update();
	}
}

void Scene::fixedUpdate()
{
	for (Layer& layer : mLayers)
	{
		layer.fixedUpdate();
	}
}

void Scene::render()
{
	for (Layer& layer : mLayers)
	{
		layer.render();
	}
}

void Scene::fontRender()
{
	for (Layer& layer : mLayers)
	{
		layer.fontRender();
	}
}

void Scene::destroy()
{
	for (Layer& layer : mLayers)
	{
		layer.destroy();
	}

}

void Scene::Enter()
{
	if (!mbInitialized)
	{
		Initialize();
		mbInitialized = true;
	}

	CreateCameras();
	mCamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mCamera->SetRotation(Vector3::Zero);
	mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	mUICamera->SetRotation(Vector3::Zero);
	{
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLight");
		directionalLight->SetRotation(Vector3(45.f, -45.f, 0.f));
		directionalLight->SetScale(Vector3(15.f, 15.f, 15.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
	}
	//Initialize();
}

void Scene::Exit()
{
	if (mDeleteObj)
	{
		for (Layer& layer : mLayers)
		{
			layer.DeleteObject();
		}
		//mDeleteObj = false;
	}

	destroy();


	mCamera = nullptr;
	mUICamera = nullptr;

	GETSINGLE(TimerMgr)->GetInstance()->ChangeScene();

	renderer::lights.clear();
}

void Scene::AddGameObject(GameObj* gameObj, eLayerType eLayer)
{
	mLayers[static_cast<UINT>(eLayer)].AddGameObject(gameObj, eLayer);
}


std::vector<GameObj*> Scene::GetDontDestroyObjects()
{
	std::vector<GameObj*> allLayerDontDestroyObjs;
	for (Layer& layer : mLayers)
	{
		std::vector<GameObj*> dontDestroyObjs = layer.GetDontDestroyObjects();

		allLayerDontDestroyObjs.insert(allLayerDontDestroyObjs.end(), dontDestroyObjs.begin(), dontDestroyObjs.end());

	}

	return allLayerDontDestroyObjs;
}

const std::vector<GameObj*>& Scene::GetGameObjects(eLayerType _eLayer)
{

	return mLayers[static_cast<UINT>(_eLayer)].GetGameObjects();
}

GameObj* Scene::GetPlayer()
{
	GameObj* player = nullptr;
	std::vector<GameObj*> playerLayer = mLayers[static_cast<UINT>(eLayerType::Player)].GetGameObjects();
	for (size_t i = 0; i < playerLayer.size(); i++)
	{
		if (L"Player" == playerLayer[i]->GetName())
		{
			player = playerLayer[i];
		}
	}

	//assert(player);

	return player;
}

void Scene::CreateCameras()
{
	{
		if (!mCamera)
		{
			// main Camera
			mCamera = object::Instantiate<GameObj>(eLayerType::Camera, this, L"MainCamera");
			mCamera->SetPos(Vector3(0.f, 5.f, -20.f));

			// main cam
			Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
			cameraComp->SmoothOn();
			cameraComp->SetProjectionType(eProjectionType::Perspective);
			cameraComp->SetLayerMaskOFF(eLayerType::UI);
			cameraComp->SetLayerMaskOFF(eLayerType::Camera);
			cameraComp->SetNear(0.01f);

			CameraScript* cameraScript = mCamera->AddComponent<CameraScript>(eComponentType::Script);

		}
		if (!mUICamera)
		{
			// UI Camera
			mUICamera = object::Instantiate<GameObj>(eLayerType::Camera, this, L"UICamera");
			mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
			Camera* cameraUIComp = mUICamera->AddComponent<Camera>(eComponentType::Camera);

			cameraUIComp->SetProjectionType(eProjectionType::Orthographic);
			cameraUIComp->SmoothOn();
			cameraUIComp->DisableLayerMasks();
			cameraUIComp->SetLayerMaskOn(eLayerType::UI);
		}
		mCamera->GetScript<CameraScript>()->SetUICameraObject(mUICamera);


		renderer::UICamera = mUICamera->GetComponent<Camera>();
		renderer::mainCamera = mCamera->GetComponent<Camera>();
	}
}
