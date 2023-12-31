#include "PlayerStateScript.h"
#include "Player.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"
#include "AudioSource.h"
#include "PlayerBGMScript.h"

PlayerStateScript::PlayerStateScript()
	: Script()
	, mStateEventList{}
	, mPlayer(nullptr)
	, mbAnimationRunning(false)
	, mbHavingCap(true)
{
	// 메모리 공간 확보
	mStateEventList.reserve(static_cast<UINT>(Player::ePlayerState::Die) + 1);

	// 이벤트 바인딩
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Idle, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Move, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Jump, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Squat, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::SquatMove, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Air, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Fall, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Wall, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Hit, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Groggy, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::ThrowCap, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::CatchCap, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Capture, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Die, this));
}

PlayerStateScript::~PlayerStateScript()
{
	mStateEventList.clear();
}

void PlayerStateScript::Update()
{
	if (mPlayer == nullptr)
		return;

	UINT iState = static_cast<UINT>(mPlayer->GetPlayerState());
	// enum 상태와 매칭되는 배열을 인덱스로 접근
	mStateEventList[iState]();


	Script::Update();
}

void PlayerStateScript::Initialize()
{
	// Owner 형 변환
	if (GetOwner())
		mPlayer = dynamic_cast<Player*>(GetOwner());

	mAnimator = mPlayer->GetComponent<BoneAnimator>();
	assert(mAnimator);

	mJumpCount = 0;

	GetPhysXRigidBody()->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	GetPhysXRigidBody()->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);

}

void PlayerStateScript::Idle()
{
	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	if (!rigidbody)
		return;
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;
	if(animator->PlayAnimationName() != L"Wait")
		animator->Play(L"Wait");
	//rigidbody->SetAirOff();
}

void PlayerStateScript::Move()
{
	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	PhysicalMovement* moveMent = GetOwner()->GetComponent<PhysicalMovement>();
	assert(moveMent);

	Physical* physical = GetOwner()->GetComponent<Physical>();
	assert(physical);



	if (mAnimator->PlayAnimationName() == L"Brake")
		return;


	if (GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::W)
		&& GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::S)
		&& GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::A)
		&& GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::D))
	{
		mAnimator->Play(L"Brake");
		//GetOwner()->GetComponent<AudioSource>()->Stop(L"FootNote");
		mPlayer->SetPlayerState(Player::ePlayerState::Idle);

		return;
	}

	Transform* tr = mPlayer->GetComponent<Transform>();
	assert(tr);


	bool able = false;
	auto Input_DownFunC = [&](eKeyCode key, eKeyCode mult_key, math::Vector3 rotation)
	{
		if (able)
			return;

		if (GETSINGLE(InputMgr)->GetKeyDown(key) || GETSINGLE(InputMgr)->GetKeyTap(key))
		{
			if (GETSINGLE(InputMgr)->GetKeyDown(mult_key) || GETSINGLE(InputMgr)->GetKeyTap(mult_key))
			{
				rotation.y += renderer::mainCamera->GetTransform()->GetRotationY();
				tr->SetPhysicalRotation(rotation);
				able = true;
			}
		}
	};


	Input_DownFunC(eKeyCode::W, eKeyCode::D, math::Vector3(0.0f, -135.f, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::A, math::Vector3(0.0f, -225, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::W, math::Vector3(0.0f, -180.f, 0.0f));

	Input_DownFunC(eKeyCode::S, eKeyCode::D, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::A, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::S, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::A, eKeyCode::A, math::Vector3(0.0f, 90.f, 0.0f));
	Input_DownFunC(eKeyCode::D, eKeyCode::D, math::Vector3(0.0f, -90.f, 0.0f));


	if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&& mAnimator->PlayAnimationName() != L"Run"
		&& mAnimator->PlayAnimationName() != L"RunStart")
	{
		mInitialForce += mForceIncrement;
		mAnimator->Play(L"RunStart");
		rigidbody->SetMaxVelocity_XZ(PLAYER_RUN_VELOCITY);
	}
	else if (
		GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&&(mAnimator->PlayAnimationName() == L"RunStart"))
	{
		mFootInterval = 0.1f;
		mInitialForce += mForceIncrement;
		rigidbody->SetMaxVelocity_XZ(PLAYER_RUN_VELOCITY);
	}
	else if (
		GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&& (mAnimator->PlayAnimationName() == L"Run"))
	{
		mInitialForce = 50000.f;
		rigidbody->SetMaxVelocity_XZ(PLAYER_RUN_VELOCITY);
	}
	else if(mAnimator->PlayAnimationName() != L"Walk")
	{
		//GetOwner()->GetComponent<AudioSource>()->Stop(L"FootNote");
		//mPlayer->GetComponent<PlayerBGMScript>()->SetSound(PlayerBGMScript::eSoundState::FootNote);
		mFootInterval = 0.47f;
		mAnimator->Play(L"Walk");
		mInitialForce = 35000.f;
		rigidbody->SetMaxVelocity_XZ(PLAYER_WALK_VELOCITY);
	}
	else
	{
		mInitialForce = 35000.f;
		rigidbody->SetMaxVelocity_XZ(PLAYER_WALK_VELOCITY);
	}
	rigidbody->AddForce(-tr->Forward() * mInitialForce * DT);


	if (mFootIntervalTime > mFootInterval)
	{
		mPlayer->GetComponent<AudioSource>()->Play(L"FootNote", false);
		mFootIntervalTime = 0;
	}

	mFootIntervalTime += DT;

}



void PlayerStateScript::Jump()
{
	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	if (rigidbody->IsOnAir())
	{
		return;
	}

	if (mAnimator->PlayAnimationName() != L"Jump"
		&& mAnimator->PlayAnimationName() != L"Jump2"
		&& mAnimator->PlayAnimationName() != L"Jump3")
	{
		if (mJumpCount == 0)
		{
			mAnimator->Play(L"Jump");
			mPlayer->GetComponent<AudioSource>()->Play(L"ha1", false);

			rigidbody->SetMaxVelocity_Y(PLAYER_JUMP_VELOCITY);
			rigidbody->ApplyGravity();
			//rigidbody->SetAirOn();
			rigidbody->AddForce(math::Vector3(0.0f, PLAYER_JUMPFORCE, 0.0f));
			mJumpCount++;
		}
		else if (mJumpCount == 1)
		{
			mAnimator->Play(L"Jump2");
			mPlayer->GetComponent<AudioSource>()->Play(L"ha2", false);

			rigidbody->SetMaxVelocity_Y(PLAYER_JUMP_VELOCITY + 2.f);
			rigidbody->ApplyGravity();
			//rigidbody->SetAirOn();
			rigidbody->AddForce(math::Vector3(0.0f, PLAYER_JUMPFORCE, 0.0f));
			mJumpCount++;

		}
		else if (mJumpCount == 2)
		{
			mAnimator->Play(L"Jump3");
			mPlayer->GetComponent<AudioSource>()->Play(L"wahoo", false);

			rigidbody->SetMaxVelocity_Y(PLAYER_JUMP_VELOCITY + 5.f);
			rigidbody->ApplyGravity();
			//rigidbody->SetAirOn();
			rigidbody->AddForce(math::Vector3(0.0f, PLAYER_JUMPFORCE, 0.0f));
  			mJumpCount = 0;
		}

	}

	if (((mAnimator->PlayAnimationName() == L"Jump" && mAnimator->IsComplete())) 
		|| ((mAnimator->PlayAnimationName() == L"Jump2" && mAnimator->IsComplete()))
		|| ((mAnimator->PlayAnimationName() == L"Jump3" && mAnimator->IsComplete()))
		||rigidbody->GetVelocity().y < 0)
	{
		mAnimator->Play(L"Fall");
		mPlayer->SetPlayerState(Player::ePlayerState::Fall);
	}
}

void PlayerStateScript::Squat()
{

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	if (mAnimator->PlayAnimationName() == L"SquatEnd")
		return;

	if (GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::Z))
	{
		mAnimator->Play(L"SquatEnd", false);
		return;
	}

	if (mAnimator->PlayAnimationName() != L"SquatStart"
		&& mAnimator->PlayAnimationName() != L"SquatWalk"
		&& mAnimator->PlayAnimationName() != L"SquatWait"
		&& mAnimator->PlayAnimationName() != L"SquatEnd"
		&& mAnimator->PlayAnimationName() != L"Rolling")
	{
		mAnimator->Play(L"SquatStart");
	}

	if (mAnimator->PlayAnimationName() == L"SquatWalk"
		|| mAnimator->PlayAnimationName() == L"Rolling")
	{
		mAnimator->Play(L"SquatWait");
	}

	if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT))
	{
		mPlayer->SetPlayerState(Player::ePlayerState::SquatMove);
		return;
	}

	if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::W)
		|| GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::S)
		|| GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::A)
		|| GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::D))
	{
		mPlayer->SetPlayerState(Player::ePlayerState::SquatMove);
		return;
	}

}

void PlayerStateScript::SquatMove()
{
	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	PhysicalMovement* moveMent = GetOwner()->GetComponent<PhysicalMovement>();
	assert(moveMent);

	Physical* physical = GetOwner()->GetComponent<Physical>();
	assert(physical);

	if (mAnimator->PlayAnimationName() == L"SquatEnd")
		return;

	if (GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::Z))
	{
		mAnimator->Play(L"SquatEnd");
		return;
	}

	if (GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::W)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::S)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::A)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::D)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::LSHIFT))
	{
		if (!GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::W)
			&& !GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::S)
			&& !GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::A)
			&& !GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::D))
		{
			mPlayer->SetPlayerState(Player::ePlayerState::Squat);
			return;
		}
	}



	Transform* tr = mPlayer->GetComponent<Transform>();
	assert(tr);


	bool able = false;
	auto Input_DownFunC = [&](eKeyCode key, eKeyCode mult_key, math::Vector3 rotation)
	{
		if (able)
			return;

		if (GETSINGLE(InputMgr)->GetKeyDown(key) || GETSINGLE(InputMgr)->GetKeyTap(key))
		{
			if (GETSINGLE(InputMgr)->GetKeyDown(mult_key) || GETSINGLE(InputMgr)->GetKeyTap(mult_key))
			{
				rotation.y += renderer::mainCamera->GetTransform()->GetRotationY();
				tr->SetPhysicalRotation(rotation);
				able = true;
			}
		}
	};


	Input_DownFunC(eKeyCode::W, eKeyCode::D, math::Vector3(0.0f, -135.f, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::A, math::Vector3(0.0f, -225, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::W, math::Vector3(0.0f, -180.f, 0.0f));

	Input_DownFunC(eKeyCode::S, eKeyCode::D, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::A, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::S, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::A, eKeyCode::A, math::Vector3(0.0f, 90.f, 0.0f));

	rigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	rigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);

	rigidbody->SetMaxVelocity_XZ(PLAYER_SQUATWALK_VELOCITY);


	if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&& mAnimator->PlayAnimationName() != L"RollingStart"
		&& mAnimator->PlayAnimationName() != L"Rolling")
	{
		mInitialForce += mForceIncrement;
		mAnimator->Play(L"RollingStart");
		rigidbody->SetMaxVelocity_XZ(PLAYER_ROLLING_VELOCITY);
	}
	else if (
		GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&& (mAnimator->PlayAnimationName() == L"RollingStart"))
	{
		mInitialForce += mForceIncrement;
		rigidbody->SetMaxVelocity_XZ(PLAYER_ROLLING_VELOCITY);
	}
	else if (
		GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&& (mAnimator->PlayAnimationName() == L"Rolling"))
	{
		mInitialForce = 50000.f;
		rigidbody->SetMaxVelocity_XZ(PLAYER_ROLLING_VELOCITY);
	}
	else if (mAnimator->PlayAnimationName() != L"SquatWalk")
	{
		mAnimator->Play(L"SquatWalk");
		mInitialForce = 35000.f;
		rigidbody->SetMaxVelocity_XZ(PLAYER_SQUATWALK_VELOCITY);
	}
	else
	{
		mInitialForce = 35000.f;
		rigidbody->SetMaxVelocity_XZ(PLAYER_SQUATWALK_VELOCITY);
	}

	rigidbody->AddForce(-tr->Forward() * mInitialForce * DT);
}

void PlayerStateScript::Air()
{
}

void PlayerStateScript::Fall()
{
	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);
	Transform* tr = mPlayer->GetComponent<Transform>();
	assert(tr);


	bool able = false;
	auto Input_DownFunC = [&](eKeyCode key, eKeyCode mult_key, math::Vector3 rotation)
	{
		if (able)
			return;

		if (GETSINGLE(InputMgr)->GetKeyDown(key) || GETSINGLE(InputMgr)->GetKeyTap(key))
		{
			if (GETSINGLE(InputMgr)->GetKeyDown(mult_key) || GETSINGLE(InputMgr)->GetKeyTap(mult_key))
			{
				rotation.y += renderer::mainCamera->GetTransform()->GetRotationY();
				tr->SetPhysicalRotation(rotation);
				able = true;
			}
		}
	};


	Input_DownFunC(eKeyCode::W, eKeyCode::D, math::Vector3(0.0f, -135.f, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::A, math::Vector3(0.0f, -225, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::W, math::Vector3(0.0f, -180.f, 0.0f));

	Input_DownFunC(eKeyCode::S, eKeyCode::D, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::A, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::S, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::A, eKeyCode::A, math::Vector3(0.0f, 90.f, 0.0f));

	//rigidbody->SetMaxVelocity_XZ(PLAYER_WALK_VELOCITY);

	rigidbody->SetMaxVelocity_Y(PLAYER_JUMP_VELOCITY + 5.f);

	rigidbody->AddForce(-tr->Forward() * mInitialForce * DT);
}

void PlayerStateScript::Wall()
{
}

void PlayerStateScript::Hit()
{
}

void PlayerStateScript::Groggy()
{
}

void PlayerStateScript::ThrowCap()
{
	if (mbHavingCap)
	{
		if (mAnimator->PlayAnimationName() != L"ThrowCap")
		{
			mPlayer->GetMarioCap()->GetPhysical()->KinematicActorWakeup();
			mAnimator->Play(L"ThrowCap", false);
			mbHavingCap = false;
		}
	}
}

void PlayerStateScript::CatchCap()
{
	if (mAnimator->PlayAnimationName() != L"CatchCap")
	{
		mAnimator->Play(L"CatchCap", false);
	}
}

void PlayerStateScript::Capture()
{
	if (!mAnimator)
		return;
	if (mAnimator->PlayAnimationName() != L"Bind")
	{
		mAnimator->Play(L"Bind");
	}
}

void PlayerStateScript::Die()
{
}


