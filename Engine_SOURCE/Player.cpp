#include "Player.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "PlayerScript.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"

namespace dru
{
	Player::Player()
	{
		SetLayerType(eLayerType::Player);

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);

		MeshRenderer* spriteRenderer = AddComponent<MeshRenderer>(eComponentType::Renderer);
	}

	Player::~Player()
	{

	}

	void Player::Initialize()
	{

		GameObj::Initialize();
	}

	void Player::update()
	{

		GameObj::update();
	}

	void Player::fixedUpdate()
	{
		GameObj::fixedUpdate();
	}

	void Player::render()
	{
		
		GameObj::render();
	}

	void Player::fontRender()
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



}