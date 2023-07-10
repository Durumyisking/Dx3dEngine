#include "Player.h"
#include "RigidBody.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "PlayerScript.h"
#include "MeshRenderer.h"

namespace dru
{
	Player::Player()
	{
		SetLayerType(eLayerType::Player);

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);

		//MeshRenderer* spriteRenderer = AddComponent<MeshRenderer>(eComponentType::Renderer);

		PlayerScript* script = AddComponent<PlayerScript>(eComponentType::Script);
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



}