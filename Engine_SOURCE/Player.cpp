#include "Player.h"
#include "MarioParts.h"
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
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	AddComponent<PlayerScript>(eComponentType::Script);
	AddComponent<Transform>(eComponentType::Transform);
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	PhysXRigidBody* rigid =AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
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

	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(0.5f, 0.5f, 0.5f));
	mMarioPhysicPos = GetComponent<Transform>()->GetPhysicalPosition();

	MarioParts* mHandL = object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mHandR = object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mHead = object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mFace = object::Instantiate<MarioParts>(eLayerType::Player);

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
	mMarioPhysicPos = GetComponent<Transform>()->GetPosition();

	for (auto i : mParts)
	{
		i->GetComponent<Transform>()->SetPosition(mMarioPhysicPos);
		i->Update();
	}

	DynamicObject::Update();
}

void Player::FixedUpdate()
{
	DynamicObject::FixedUpdate();
}

void Player::Render()
{
		
	DynamicObject::Render();
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

void Player::BoneInitialize()
{
}

