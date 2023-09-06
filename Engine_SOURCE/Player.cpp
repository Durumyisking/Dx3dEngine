#include "Player.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "PlayerScript.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"



Player::Player()
{
	SetLayerType(eLayerType::Player);

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);

	MeshRenderer* meshRenderer = AddComponent<MeshRenderer>(eComponentType::Renderer);
}

Player::~Player()
{

}

void Player::Initialize()
{

	DynamicObject::Initialize();
}

void Player::Update()
{

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

