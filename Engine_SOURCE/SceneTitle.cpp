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
	CSceneTitle::CSceneTitle()
		:  mCamera(nullptr)

	{
	}

	CSceneTitle::~CSceneTitle()
	{

	}

	void CSceneTitle::Initialize()
	{
		
		
		CScene::Initialize();
	}

	void CSceneTitle::update()
	{

	
		CScene::update();
	}

	void CSceneTitle::fixedUpdate()
	{
		CScene::fixedUpdate();
	}

	void CSceneTitle::render()
	{
		
	}

	void CSceneTitle::Enter()
	{
		//mDeleteObj = true;

		{
			// main Ä«¸Þ¶ó
			mCamera = object::Instantiate<CGameObj>(eLayerType::Camera, L"MainCam");
			CCamera* cameraComp = mCamera->AddComponent<CCamera>(eComponentType::Camera);
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			cameraComp->SmoothOn();
			mCamera->AddComponent<CCameraScript>(eComponentType::Script);
			renderer::mainCamera = cameraComp;
			cameraComp->SetProjectionType(eProjectionType::Perspective);
			mCamera->SetPos(Vector3(0.f, 0.f, -20.f));

		}
		

		{
			CGameObj* directionalLight = object::Instantiate<CGameObj>(eLayerType::None, this, L"DirectionalLightTitleScene");
			directionalLight->SetPos({ 0.f, 0.f, -100.f });
			CLight* lightComp = directionalLight->AddComponent<CLight>(eComponentType::Light);
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse({ 1.f, 1.f, 1.f, 1.f });
		}

		{
			CPlayer* player = object::Instantiate<CPlayer>(eLayerType::Player);
			player->SetPos(Vector3(0.0f, 0.0f, 10.0f));
			player->SetName(L"Player");
			player->GetComponent<CSpriteRenderer>()->SetMaterialByKey(L"BasicMaterial");
		}



		CScene::Enter();
	}

	void CSceneTitle::Exit()
	{
		CScene::Exit();
	}


}