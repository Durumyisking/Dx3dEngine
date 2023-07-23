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
#include "Model.h"

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

#include "Sphere.h"
#include "Box.h"


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
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Mario");
		//model->Test();
		const Model::ModelNode* node = model->FindNode(L"nw4f_root");
		const Model::ModelNode* node1 = model->FindNode(L"JointRoot");
		const Model::ModelNode* node2 = model->FindNode(L"Hip");
		const Model::ModelNode* node3 = model->FindNode(L"LegL1");
		const Model::ModelNode* node4 = model->FindNode(L"LegL2");
		const Model::ModelNode* node5 = model->FindNode(L"FootL");

		/*0 "nw4f_root" - 1
			1 "AllRoot" 0
			2 "JointRoot" 1
			3 "Hip" 2
			4 "LegL1" 3
			5 "LegL2" 4
			6 "FootL" 5
			7 "ToeL" 6
			8 "LegR1" 3
			9 "LegR2" 8
			10 "FootR" 9
			11 "ToeR" 10
			12 "Spine1" 2
			13 "Spine2" 12
			14 "Head" 13
			15 "ShoulderL" 13
			16 "ArmL1" 15
			17 "ArmL1Sub" 16
			18 "ArmL2" 16
			19 "ArmL2Sub" 18
			20 "HandL" 18
			21 "ShoulderR" 13
			22 "ArmR1" 21
			23 "ArmR1Sub" 22
			24 "ArmR2" 22
			25 "ArmR2Sub" 24
			26 "HandR" 24
			27 "Mario" 0*/

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
			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 100.f, 0.f));
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
			player->SetPos(Vector3(0.f, 0.f, 0.f));
			player->SetScale({ 1, 1.f, 1.f });
			player->SetName(L"Player");
			player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
			//player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
			player->GetComponent<MeshRenderer>()->SetMeshByKey(L"test0");

			for (int i = 1; i < 14; ++i)
			{
				Player* player = object::Instantiate<Player>(eLayerType::Player);
				player->SetPos(Vector3(0.f, 0.f, 0.f));
				player->SetScale({ 1, 1.f, 1.f });
				player->SetName(L"Player");
				player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
				//player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
				player->GetComponent<MeshRenderer>()->SetMeshByKey(L"test" + std::to_wstring(i));
			}

			//Player* player1 = object::Instantiate<Player>(eLayerType::Player);
			//player1->SetPos(Vector3(0.f, 0.f, 0.f));
			//player1->SetScale({ 1, 1.f, 1.f });
			//player1->SetName(L"Player");
			//player1->GetComponent<MeshRenderer>()->SetMaterialByKey(L"FlatMaterial");
			////player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
			//player1->GetComponent<MeshRenderer>()->SetMeshByKey(L"test1");
			//player1->GetComponent<Transform>()->SetParent(player);

			//Player* player2 = object::Instantiate<Player>(eLayerType::Player);
			//player2->SetPos(Vector3(0.f, 0.f, 0.f));
			//player2->SetScale({ 1, 1.f, 1.f });
			//player2->SetName(L"Player");
			//player2->GetComponent<MeshRenderer>()->SetMaterialByKey(L"FlatMaterial");
			////player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
			//player2->GetComponent<MeshRenderer>()->SetMeshByKey(L"test2");
			//player2->GetComponent<Transform>()->SetParent(player);
			
			//player->AddComponent<PlayerScript>(eComponentType::Script);

			//Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
			//physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(2.5f, 2.5f, 2.5f));
			//PxRigidDynamic* dy = physical->GetActor<PxRigidDynamic>();

			//PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
			////rigid->RemoveGravity();

			//player->AddComponent<PhysXCollider>(eComponentType::Collider);
			//player->AddComponent<PhysicalMovement>(eComponentType::Movement);
		}

		{
			
			Sphere* sphere = object::Instantiate<Sphere>(eLayerType::PhysicalObject);
			sphere->SetPos(Vector3(-5.f, 20.f, 5.f));
			sphere->SetScale({ 2.5f, 2.5f, 2.5f });
			sphere->SetName(L"Sphere");
			Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"dirt_color", L"dirt_normal", L"PhongShader", L"mat_dirt");
			// sphere->GetComponent<MeshRenderer>()->SetMaterial(mat);

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
			plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(50.f, 0.25f, 50.f));

			PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

			plane->AddComponent<PhysXCollider>(eComponentType::Collider);
		}

		Scene::Enter();
	}

	void SceneTitle::Exit()
	{
		Scene::Exit();
	}


}