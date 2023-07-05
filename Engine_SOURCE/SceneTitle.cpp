#include "SceneTitle.h"
#include "TimeMgr.h"

#include "FadeScript.h"
#include "Object.h"
#include "Camera.h"
#include "Input.h"
#include "Collider2D.h"
#include "Animator.h"

#include "AudioSource.h"



namespace dru
{
	SceneTitle::SceneTitle()
		:  mCamera(nullptr)

	{
	}

	SceneTitle::~SceneTitle()
	{

	}

	void SceneTitle::Initialize()
	{
		
		
		Scene::Initialize();
	}

	void SceneTitle::update()
	{

	
		Scene::update();
	}

	void SceneTitle::fixedUpdate()
	{
		Scene::fixedUpdate();
	}

	void SceneTitle::render()
	{
		
	}

	void SceneTitle::Enter()
	{
		//mDeleteObj = true;

		{
			// main Ä«¸Þ¶ó
			mCamera = object::Instantiate<GameObj>(eLayerType::Camera, L"MainCam");
			Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			cameraComp->SmoothOn();
			mCamera->AddComponent<CameraScript>(eComponentType::Script);
			renderer::mainCamera = cameraComp;
			cameraComp->SetProjectionType(eProjectionType::Perspective);
			mCamera->SetPos(Vector3(0.f, 0.f, -20.f));

		}
		
		{
			GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLightTitleScene");
			directionalLight->SetPos({ 0.f, 0.f, -100.f });
			Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse({ 1.f, 1.f, 1.f, 1.f });
		}

		{
			Player* player = object::Instantiate<Player>(eLayerType::Player);
			player->SetPos(Vector3(0.0f, 0.0f, 10.0f));
			player->SetName(L"Player");
			player->GetComponent<SpriteRenderer>()->SetMaterialByKey(L"BasicMaterial");
		}



		Scene::Enter();
	}

	void SceneTitle::Exit()
	{
		Scene::Exit();
	}


}