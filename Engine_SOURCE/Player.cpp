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

	GameObj::Initialize();
}

void Player::Update()
{
	GameObj::Update();
}

void Player::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void Player::Render()
{
		
	GameObj::Render();
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

