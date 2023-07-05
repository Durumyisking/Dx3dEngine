#include "Player.h"
#include "CameraScript.h"
#include "GameObj.h"
#include "RigidBody.h"
#include "Input.h"
#include "TimeMgr.h"

namespace dru
{
	Player::Player()
	{
		SetLayerType(eLayerType::Player);

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);

		SpriteRenderer* spriteRenderer = AddComponent<SpriteRenderer>(eComponentType::Renderer);

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