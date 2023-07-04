#include "Player.h"
#include "CameraScript.h"
#include "GameObj.h"
#include "RigidBody.h"
#include "Input.h"
#include "TimeMgr.h"

namespace dru
{
	CPlayer::CPlayer()
	{
		SetLayerType(eLayerType::Player);
		SetScale(Vector3(1.25f, 1.25f, 1.f));

//		CRigidBody* rigidbody = this->AddComponent<CRigidBody>(eComponentType::RigidBody);

		CSpriteRenderer* SpriteRenderer = AddComponent<CSpriteRenderer>(eComponentType::Renderer);

	}

	CPlayer::~CPlayer()
	{

	}

	void CPlayer::Initialize()
	{

		CGameObj::Initialize();
	}

	void CPlayer::update()
	{

		CGameObj::update();
	}

	void CPlayer::fixedUpdate()
	{
		CGameObj::fixedUpdate();
	}

	void CPlayer::render()
	{
		
		CGameObj::render();
	}

	void CPlayer::fontRender()
	{



	}



}