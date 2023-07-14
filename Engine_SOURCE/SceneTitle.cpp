#include "SceneTitle.h"
#include "TimeMgr.h"

#include "Object.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Texture.h"
#include "Camera.h"
#include "CameraScript.h"
#include "FontWrapper.h"

#include "GridScript.h"

#include "Application.h"
#include "Player.h"

#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PlayerScript.h"
#include "PhysicalMovement.h"
#include "PhysicsMgr.h"
#include "PhysicsScene.h"
#include "PhysX.h"


extern dru::Application application;


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
		GETSINGLE(PhysXCollisionMgr)->SetCollisionGroup(eLayerType::Platforms, eLayerType::Player);
		
		Scene::Initialize();
	}

	void SceneTitle::update()
	{
		//PxMaterial* mat = GETSINGLE(PhysicsMgr)->GetEnvironment()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);

		//GETSINGLE(PhysicsMgr)->GetEnvironment()->GetPhysicsScene()->CreateDynamic(PxTransform(PxVec3(rand() % 8, 4 * (rand() % 5), 20)), PxSphereGeometry(0.5f), *mat, PxVec3(0, -30, -60));

	
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
			mCamera = object::Instantiate<GameObj>(eLayerType::Camera, L"MainCam");
			Camera* cameraComp = mCamera->AddComponent<Camera>(eComponentType::Camera);
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			cameraComp->SmoothOn();
			mCamera->AddComponent<CameraScript>(eComponentType::Script);
			renderer::mainCamera = cameraComp;
			cameraComp->SetProjectionType(eProjectionType::Perspective);
			mCamera->SetPos(Vector3(0.f, 15.f, 0.f));

		}


		{
			GameObj* gridObject = object::Instantiate<GameObj>(eLayerType::Grid, L"Grid");
		
			dru::MeshRenderer* gridMr = gridObject->AddComponent<dru::MeshRenderer>(eComponentType::MeshRenderer);

			gridMr->SetMesh(dru::GETSINGLE(ResourceMgr)->Find<dru::Mesh>(L"Gridmesh"));
			gridMr->SetMaterial(dru::GETSINGLE(ResourceMgr)->Find<Material>(L"GridMaterial"));
			gridMr->LODOff();

			dru::GridScript* gridScript = gridObject->AddComponent<dru::GridScript>(eComponentType::Script);
			gridScript->SetCamera(mainCamera);

			float w = static_cast<float>(application.GetWidth());
			float h = static_cast<float>(application.GetHeight());
			gridObject->SetPos({ 0.f, 0.f, 0.f });
			gridObject->SetScale(Vector3(1.f, 1.f, 1.f));
		}
		
		{
			GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLightTitleScene");
			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 10.f, 0.f));
			Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
			lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
			lightComp->SetAmbient(Vector4(0.5f, 0.5f, 0.5f, 1.f));
		}

		{
			GameObj* pointLight = object::Instantiate<GameObj>(eLayerType::None, this, L"PointLightTitleScene");
			pointLight->GetComponent<Transform>()->SetPosition(Vector3(2.5f, 10.f, 0.f));
			Light* lightComp = pointLight->AddComponent<Light>(eComponentType::Light);
			lightComp->SetType(eLightType::Point);
			lightComp->SetDiffuse(Vector4(0.f, 1.f, 1.f, 1.f));
			lightComp->SetRadius(10.f);
		}

		{
			Player* player = object::Instantiate<Player>(eLayerType::Player);
			player->SetPos(Vector3(5.f, 5.f, 5.f));
			player->SetName(L"Player");
			player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"FlatMaterial");
			player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
			player->SetScale({ 5.f, 5.f, 5.f });
		}

		{
			
			Player* player = object::Instantiate<Player>(eLayerType::Player);
			player->SetPos(Vector3(-5.f, 5.f, 5.f));
			player->SetScale({ 5.f, 5.f, 5.f });
			player->SetName(L"Player");
			player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
			player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
			player->AddComponent<PlayerScript>(eComponentType::Script);

			Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
			physical->InitialPhysics(eActorType::Dynamic, eGeometryType::Sphere, Vector3(2.5f, 2.5f, 2.5f));
			PxRigidDynamic* dy =  physical->GetActor<PxRigidDynamic>();

			PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
			//rigid->RemoveGravity();
			player->AddComponent<PhysXCollider>(eComponentType::Collider);
			player->AddComponent<PhysicalMovement>(eComponentType::Movement);
		}

		{
			GameObj* pointLight = object::Instantiate<GameObj>(eLayerType::None, this, L"PointLightTitleScene");
			pointLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 1.f, 5.f));
			Light* lightComp = pointLight->AddComponent<Light>(eComponentType::Light);
			lightComp->SetType(eLightType::Point);
			lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
			lightComp->SetRadius(20.f);
		}

		{
			GameObj* plane = object::Instantiate<GameObj>(eLayerType::Platforms);
			plane->SetPos(Vector3(0.f, -0.251f, 0.f));
			plane->SetScale({ 100.f, 0.5f, 100.f });
			plane->SetName(L"Plane");
			plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"PhongMaterial");
			plane->AddComponent<Physical>(eComponentType::Physical)->InitialPhysics(eActorType::Static, eGeometryType::Box, Vector3(50.f, 0.25f, 50.f));

			PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
			rigid->RemoveGravity();

			plane->AddComponent<PhysXCollider>(eComponentType::Collider);
		}

		Scene::Enter();
	}

	void SceneTitle::Exit()
	{
		Scene::Exit();
	}


}