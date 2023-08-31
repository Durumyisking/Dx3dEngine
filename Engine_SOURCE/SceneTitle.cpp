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
#include "Model.h"
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

#include "Sphere.h"
#include "Box.h"

#include "ServerMgr.h"

#include "InputMgr.h"

#include "SkyBox.h"
#include "BoneAnimator.h"

#include "Panal.h"

extern Application application;


SceneTitle::SceneTitle()
	: mCamera(nullptr)

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
	if (KEY_TAP(N_9))
	{
		GETSINGLE(server::ServerMgr)->OpenServer();
	}
	if (KEY_TAP(N_0))
	{
		GETSINGLE(server::ServerMgr)->ConnectAsClient();
	}


	for (auto& i : mRoot)
	{
		i->GetComponent<Transform>()->SetPosition(mRootGameobj->GetComponent<Transform>()->GetPosition());
		i->GetComponent<Transform>()->SetRotation(mRootGameobj->GetComponent<Transform>()->GetRotation());
	}

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
		mCamera->SetPos(Vector3(0.f, 5.f, -20.f));

	}

	//{
	//	// UI Camera
	//	mUICamera = object::Instantiate<GameObj>(eLayerType::Camera);
	//	Camera* cameraUIComp = mUICamera->AddComponent<Camera>(eComponentType::Camera);
	//	mUICamera->AddComponent<CameraScript>(eComponentType::Script);

	//	cameraUIComp->SetProjectionType(eProjectionType::Perspective);
	//	cameraUIComp->SmoothOn();
	//	cameraUIComp->DisableLayerMasks();
	//	cameraUIComp->TurnLayerMask(eLayerType::UI, true);
	//	mUICamera->SetPos(Vector3(0.f, 5.f, -20.f));
	//}

	
	{
		/*SkyBox* box = object::Instantiate<SkyBox>(eLayerType::BackGround, L"TitleSkyBox");
		box->TextureLoad(L"TitleSky", L"..//Resources//SkyCityNightStar_color.png");*/


	}



	{
		GameObj* gridObject = object::Instantiate<GameObj>(eLayerType::Grid, L"Grid");

		MeshRenderer* gridMr = gridObject->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

		gridMr->SetMesh(GETSINGLE(ResourceMgr)->Find<Mesh>(L"Gridmesh"));
		gridMr->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(L"GridMaterial"));
		gridMr->LODOff();

		GridScript* gridScript = gridObject->AddComponent<GridScript>(eComponentType::Script);
		gridScript->SetCamera(mainCamera);

		float w = static_cast<float>(application.GetWidth());
		float h = static_cast<float>(application.GetHeight());
		gridObject->SetPos({ 0.f, 0.f, 0.f });
		gridObject->SetScale(Vector3(1.f, 1.f, 1.f));
	}

	{
		GameObj* directionalLight = object::Instantiate<GameObj>(eLayerType::None, this, L"DirectionalLightTitleScene");
		directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.f, 500.f, -1000.f));
		directionalLight->SetRotation(Vector3(45.f, 0.f, 0.f));
		directionalLight->SetScale(Vector3(15.f, 15.f, 15.f));
		Light* lightComp = directionalLight->AddComponent<Light>(eComponentType::Light);
		lightComp->SetType(eLightType::Directional);
		lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		lightComp->SetSpecular(Vector4(1.f, 1.f, 1.f, 1.f));
//		lightComp->SetAmbient(Vector4(0.5f, 0.5f, 0.5f, 1.f));
		//MeshRenderer* mr = directionalLight->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
		//mr->SetMaterialByKey(L"SunMaterial");
		//mr->ChangeColor(Vector4(1.f, 1.f, 1.f, 1.f));
	}
	

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(5.f, 5.f, 5.f));
		player->SetScale(Vector3(1.f, 1.f, 1.f));
		player->SetName(L"Player");
		Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial
		(
			L"check_albedo",
			L"check_normal", 
			L"check_metallic", 
			L"check_roughness", 
			L"PBRShader",
			L"mat_dirt"
		);
		player->GetComponent<MeshRenderer>()->SetMaterial(mat);

		//player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Capsulemesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
	}

	//Ä¸½¶ Å×½ºÆ®¿ë ÄÚµå
	{
		GameObj* capsule = object::Instantiate<GameObj>(eLayerType::Monster);
		capsule->SetPos(Vector3(500.f, 0.f, 0.f));
		capsule->SetScale(Vector3(100.0f, 100.0f, 100.0f));
		capsule->SetName(L"Capsule");
		capsule->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMeshByKey(L"Capsulemesh");
		capsule->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"FlatMaterial");

		capsule->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Capsule, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = capsule->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		capsule->AddComponent<PhysXCollider>(eComponentType::Collider);
	}


	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		player->SetName(L"Player");
		
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Mario");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);

		//animator->LoadAnimations(L"..//Resources/MarioBody/Animation");
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioBody/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioBody/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioBody/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioBody/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

<<<<<<< HEAD
		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Capsulemesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Capsule, Vector3(0.5f, 0.5f, 0.5f));
=======
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));
>>>>>>> c22fd52be324c63ef46a67bcee5e505d422e9998

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);
<<<<<<< HEAD
=======
		mRootGameobj = player;
	

>>>>>>> c22fd52be324c63ef46a67bcee5e505d422e9998
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHandL");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHandL/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioHandL/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioHandL/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioHandL/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_HandL");
		model->SetTargetBone(L"Armature_HandL");
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, 0.0f));
		mRoot.push_back(player);
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHandR");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHandR/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioHandR/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioHandR/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioHandR/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_HandR");
		model->SetTargetBone(L"Armature_HandR");
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, 0.0f));
		mRoot.push_back(player);
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHead/Animation/Defalut.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_Head");
		model->SetTargetBone(L"Armature_MarioHead");

		model->SetOffsetRotation(Vector3(0.0f, 1.570796f , -1.570796f * 2));
		mRoot.push_back(player);
	}

	{
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioFace");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioFace/Animation/AreaWaitStink.smd", 0.02f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioFace/Animation/Walk.smd", 0.02f);
		animator->Play(L"test2");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_Head");
		model->SetTargetBone(L"Armature_Face");

		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, -1.570796f * 2));
		mRoot.push_back(player);
	}

	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-50.f, -50.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioFace");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioFace/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioFace/Animation/Jump.smd", 0.05f);
		animator->Play(L"test2");*/
	}

	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-100.f, -100.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"BirdCity");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/BirdCity/Animation/FlyLanding.smd", 0.05f);
		animator->Play(L"test");*/
	}

	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CapMan");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/CapMan/Animation/Give.smd", 0.05f);
		animator->Play(L"test");*/
	}

	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(0.f, 0.f, 0.f));
		player->SetScale(Vector3(1.0f, 1.f, 1.0f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"goomba");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/goomba/Animation/Jump.smd", 0.05f);
		animator->Play(L"test");*/
	}

	{
		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-100.f, -100.f, 0.f));
		player->SetScale(Vector3(0.3f, 0.3f, 0.3f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Tank");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/Tank/Animation/Move.smd", 0.05f);
		animator->Play(L"test");*/
	}
	
	{

		/*Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(-100.f, -100.f, 0.f));
		player->SetScale(Vector3(0.3f, 0.3f, 0.3f));
		player->SetName(L"Player");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWomans");
		player->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = player->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/CityWomans/Animation/ByeBye.smd", 0.05f);
		animator->Play(L"test");*/

	}
	
	{
		GameObj* plane = object::Instantiate<GameObj>(eLayerType::Platforms);
		plane->SetPos(Vector3(0.f, -0.251f, 0.f));
		plane->SetScale({ 1000.f, 0.5f, 1000.f });
		plane->SetName(L"Plane");
		plane->AddComponent<MeshRenderer>(eComponentType::MeshRenderer)->SetMaterialByKey(L"PhongMaterial");
		plane->AddComponent<Physical>(eComponentType::Physical)->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, Vector3(500.f, 0.25f, 500.f));

		PhysXRigidBody* rigid = plane->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		plane->AddComponent<PhysXCollider>(eComponentType::Collider);
	}

	// DebugTest
	{ 
		//deferred
		//{
		//	gameobj* test2 = object::instantiate<gameobj>(elayertype::objects);
		//	test2->setpos(vector3(-10.f, 5.f, 0.f));
		//	test2->setscale({ 5.f, 5.f, 5.f });
		//	test2->setname(l"test2");

		//	test2->addcomponent<meshrenderer>(ecomponenttype::meshrenderer);
		//	meshrenderer* testrender = test2->getcomponent<meshrenderer>();
		//	material* testmaterial = getsingle(resourcemgr)->find<material>(l"deferredmaterial");

		//	testrender->setmaterial(testmaterial);
		//	testrender->setmeshbykey(l"cubemesh");
		//}		
	}
	
 //   GameObj* test1 = object::Instantiate<GameObj>(eLayerType::Objects);
	//test1->SetPos(Vector3(-5.f, 5.f, 0.f));
	//test1->SetScale({ 5.f, 5.f, 5.f });

	//test1->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	//MeshRenderer* testRender = test1->GetComponent<MeshRenderer>();

	////SpriteRenderer* spriteRender = test1->AddComponent<SpriteRenderer>(eComponentType::UI);
	////Texture* titleTexture = (GETSINGLE(ResourceMgr)->Find<Texture>(L"MarioTitle"));

	//Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"RectMesh");
	//testRender->SetMesh(mesh);
	//Material* mat = GETSINGLE(ResourceMgr)->Find<Material>(L"UIMaterial");
	////mat->SetTexture(titleTexture);
	//testRender->SetMaterial(mat);
	//testRender->SetMeshByKey(L"Cubemesh");

	Panal* testObj = object::Instantiate<Panal>(eLayerType::Objects);
	testObj->SetPos(Vector3(0.f, 5.f, 0.f));
	testObj->SetScale({ 5.f, 5.f, 5.f });
	Material* testMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"SpriteMaterial");
	Texture* titleTexture = (GETSINGLE(ResourceMgr)->Find<Texture>(L"MarioTitle"));
	testMaterial->SetTexture(titleTexture);
	testObj->GetComponent <SpriteRenderer>()->SetMaterial(testMaterial);
	testObj->GetComponent<SpriteRenderer>()->SetMaterialByKey(L"SpriteMaterial");
	testObj->GetComponent<SpriteRenderer>()->SetMeshByKey(L"Rectmesh");



	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}
