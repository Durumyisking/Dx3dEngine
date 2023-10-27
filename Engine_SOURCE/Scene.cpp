#include "Scene.h"
#include "InputMgr.h"
#include "Layer.h"
#include "Renderer.h"
#include "Object.h"
#include "CameraScript.h"
#include "Player.h"
#include "TimerMgr.h"
#include "AudioListener.h"


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

void Scene::Save(FILE* File)
{
	for (UINT i = 0; i < static_cast<UINT>(eLayerType::End); i++)
	{
		eLayerType layerType = static_cast<eLayerType>(i);
		fwrite(&layerType, sizeof(eLayerType), 1, File);

		if (layerType == eLayerType::Default || layerType == eLayerType::Camera
			 || layerType == eLayerType::UI 
			 || layerType == eLayerType::PostProcess || layerType == eLayerType::Player )
		{
			int	ObjCount = 0;
			fwrite(&ObjCount, sizeof(int), 1, File);
			continue;
		}

		std::vector<GameObj*> gameObjs = mLayers[i].GetGameObjects();
		int	ObjCount = static_cast<int>(gameObjs.size());
		fwrite(&ObjCount, sizeof(int), 1, File);

		for (GameObj* obj : gameObjs)
		{
			std::string	ClassTypeName = obj->GetObjectTypeName();

			int Length = static_cast<int>(ClassTypeName.length());

			fwrite(&Length, sizeof(int), 1, File);
			fwrite(ClassTypeName.c_str(), 1, Length, File);

			obj->Save(File);
		}
	}
}

void Scene::Load(FILE* File)
{
	for (UINT i = 0; i < static_cast<UINT>(eLayerType::End); i++)
	{
		eLayerType layerType;
		fread(&layerType, sizeof(eLayerType), 1, File);

		int	ObjCount = 0;
		fread(&ObjCount, sizeof(int), 1, File);

		for (int j = 0; j < ObjCount; ++j)
		{
			int Length = 0;
			char	ObjClassTypeName[256] = {};

			fread(&Length, sizeof(int), 1, File);
			fread(ObjClassTypeName, 1, Length, File);

			GameObj* ObjCDO = GameObj::FindObjectCDO(ObjClassTypeName);
			GameObj* NewObj = ObjCDO->Clone();
			NewObj->Load(File);

			mLayers[i].AddGameObject(NewObj, layerType);
		}
	}
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
	{
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::Default, this, L"DirectionalLight");
		directionalLight->SetPos({ 0.f, 25.f, 0.f });
		directionalLight->SetRotation(Vector3(90.f, 0.f, 0.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetRadius(0.02f);
	}

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

void Scene::SaveLayerObjects(FILE* File, eLayerType type)
{
	std::vector<GameObj*> gameObjectVec = mLayers[static_cast<UINT>(type)].GetGameObjects();

	int	ObjCount = static_cast<int>(gameObjectVec.size());
	fwrite(&ObjCount, sizeof(int), 1, File);

	for (GameObj* obj : gameObjectVec)
	{
		std::string	ClassTypeName = obj->GetObjectTypeName();

		int Length = static_cast<int>(ClassTypeName.length());

		fwrite(&Length, sizeof(int), 1, File);
		fwrite(ClassTypeName.c_str(), 1, Length, File);

		obj->Save(File);
	}
}

void Scene::LoadLayerObjects(FILE* File, eLayerType type)
{
	std::vector<GameObj*> gameObjectVec = mLayers[static_cast<UINT>(type)].GetGameObjects();

	int	ObjCount = 0;
	fread(&ObjCount, sizeof(int), 1, File);

	for (int j = 0; j < ObjCount; ++j)
	{
		int Length = 0;
		char	ObjClassTypeName[256] = {};

		fread(&Length, sizeof(int), 1, File);
		fread(ObjClassTypeName, 1, Length, File);

		GameObj* ObjCDO = GameObj::FindObjectCDO(ObjClassTypeName);
		GameObj* NewObj = ObjCDO->Clone();
		NewObj->Load(File);

		mLayers[static_cast<UINT>(type)].PushAddedObject(NewObj, type);
	}
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

GameObj* Scene::FindSceneGameObject(const std::wstring& name)
{
	GameObj* findObj = nullptr;


	for (Layer& layer : mLayers)
	{
		std::vector<GameObj*> layerObjs = layer.GetGameObjects();

		for (GameObj* Obj : layerObjs)
		{
			if (nullptr == Obj)
				continue;

			if (name == Obj->GetName())
				findObj = Obj;
		}
	}

	return findObj;
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

			mCamera->AddComponent<AudioListener>(eComponentType::AudioListener);

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
