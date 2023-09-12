#include "Player.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "PlayerScript.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"
#include "BoneAnimator.h"
#include "Object.h"


Player::Player()
{
	SetLayerType(eLayerType::Player);

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);


}

Player::~Player()
{

}

void Player::Initialize()
{
	//마리오 body 초기화
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	AddComponent<PlayerScript>(eComponentType::Script);
	AddComponent<Transform>(eComponentType::Transform);
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	AddComponent<PhysXCollider>(eComponentType::Collider);
	AddComponent<PhysicalMovement>(eComponentType::Movement);
	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);

	SetPos(Vector3(0.f, 0.f, 0.f));
	SetScale(Vector3(1.0f, 1.0f, 1.0f));
	SetName(L"Player");

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Mario");
	GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

	animator->CreateAnimation(L"test", L"..//..//Resources/MarioBody/Animation/Walk.smd", 0.05f);
	animator->CreateAnimation(L"test2", L"..//..//Resources/MarioBody/Animation/Jump.smd", 0.05f);
	animator->CreateAnimation(L"test3", L"..//..//Resources/MarioBody/Animation/Dead.smd", 0.05f);
	animator->CreateAnimation(L"test4", L"..//..//Resources/MarioBody/Animation/Run.smd", 0.05f);
	animator->Play(L"test");

	GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");

	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(1.0f, 1.0f, 1.0f));

	//마리오 파츠 관리
	MarioParts* mHandL = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mHandR = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mHead = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mFace = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);

	mHandL->SetName(L"HandL");
	mHandR->SetName(L"HandR");
	mHead->SetName(L"Head");
	mFace->SetName(L"Face");

	mParts.push_back(mHandL);
	mParts.push_back(mHandR);
	mParts.push_back(mHead);
	mParts.push_back(mFace);


	for (auto i : mParts)
	{
		i->Initialize();
	}

	DynamicObject::Initialize();
}

void Player::Update()
{

	DynamicObject::Update();

	for (auto i : mParts)
	{
		i->Update();
	}

}

void Player::FixedUpdate()
{
	DynamicObject::FixedUpdate();

	for (auto i : mParts)
	{
		i->FixedUpdate();
	}


	for (auto i : mParts)
	{
		i->GetComponent<Transform>()->SetWorldMatrix(GetComponent<Transform>()->GetWorldMatrix());
	}


}

void Player::Render()
{
	DynamicObject::Render();

	for (auto i : mParts)
	{
		i->Render();
	}

}

void Player::FontRender()
{
}

void Player::OnCollisionEnter(GameObj* gameObject)
{


}

void Player::OnTriggerEnter(GameObj* gameObject)
{
	PhysXRigidBody* rigid = GetComponent<PhysXRigidBody>();
	rigid->RemoveGravity();
	rigid->SetVelocity(AXIS::Y, 0.f);
}

void Player::OnTriggerExit(GameObj* gameObject)
{
}

