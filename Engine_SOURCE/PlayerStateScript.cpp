#include "PlayerStateScript.h"
#include "Player.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"

PlayerStateScript::PlayerStateScript()
	: Script()
	, mStateEventList{}
	, mPlayer(nullptr)
	, mbAnimationRunning(false)
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
}

void PlayerStateScript::Idle()
{
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;
	if(!(animator->PlayAnimationName()== L"Wait"))
		animator->Play(L"Wait");

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

	if (GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::UP)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::DOWN)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::LEFT)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::RIGHT))
	{
		if (!GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::UP)
			&& !GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::DOWN)
			&& !GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LEFT)
			&& !GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::RIGHT))
		{
			mAnimator->Play(L"Brake");
			mPlayer->SetPlayerState(Player::ePlayerState::Idle);
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

		if (GETSINGLE(InputMgr)->GetKeyDown(key))
		{
			if (GETSINGLE(InputMgr)->GetKeyDown(mult_key))
			{
				tr->SetPhysicalRotation(rotation);
				able = true;
			}
		}
	};

	Input_DownFunC(eKeyCode::UP, eKeyCode::RIGHT, math::Vector3(0.0f, -135.f, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::LEFT, math::Vector3(0.0f, -225, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::UP, math::Vector3(0.0f, -180.f, 0.0f));

	Input_DownFunC(eKeyCode::DOWN, eKeyCode::RIGHT, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::LEFT, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::DOWN, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::LEFT, eKeyCode::LEFT, math::Vector3(0.0f, 90.f, 0.0f));
	Input_DownFunC(eKeyCode::RIGHT, eKeyCode::RIGHT, math::Vector3(0.0f, -90.f, 0.0f));

	rigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	rigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);



	if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&& mAnimator->PlayAnimationName() != L"Run"
		&& mAnimator->PlayAnimationName() != L"RunStart")
	{
		mInitialForce += mForceIncrement;
		mAnimator->Play(L"RunStart");
		rigidbody->SetMaxVelocity(PLAYER_RUN_VELOCITY);
	}
	else if (
		GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&&(mAnimator->PlayAnimationName() == L"RunStart"))
	{
		mInitialForce += mForceIncrement;
		rigidbody->SetMaxVelocity(PLAYER_RUN_VELOCITY);
	}
	else if (
		GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LSHIFT)
		&& (mAnimator->PlayAnimationName() == L"Run"))
	{
		mInitialForce == 10000.f;
		rigidbody->SetMaxVelocity(PLAYER_RUN_VELOCITY);
	}
	else if(mAnimator->PlayAnimationName() != L"Walk")
	{
		mAnimator->Play(L"Walk");
		mInitialForce = 7000.f;
		rigidbody->SetMaxVelocity(PLAYER_WALK_VELOCITY);
	}
	else
	{
		mInitialForce = 7000.f;
		rigidbody->SetMaxVelocity(PLAYER_WALK_VELOCITY);
	}
	rigidbody->AddForce(-tr->Forward() * mInitialForce * DT);

}



void PlayerStateScript::Jump()
{
	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	if (mAnimator->PlayAnimationName() != L"Jump")
	{
		mAnimator->Play(L"Jump", false);

		rigidbody->SetMaxVelocity_Y(10.f);
		rigidbody->ApplyGravity();
		rigidbody->SetAirOn();
		rigidbody->AddForce(math::Vector3(0.0f, PLAYER_JUMPFORCE, 0.0f));
	}

	if ((mAnimator->PlayAnimationName() == L"Jump" && mAnimator->IsComplete()) 
		||rigidbody->GetVelocity().y < 0)
	{
		mAnimator->Play(L"Fall", false);
		mPlayer->SetPlayerState(Player::ePlayerState::Fall);
	}
}

void PlayerStateScript::Squat()
{

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	if (mAnimator->PlayAnimationName() != L"SquatStart"
		&& mAnimator->PlayAnimationName() != L"SquatMove"
		&& mAnimator->PlayAnimationName() != L"SquatWait")
	{
		mAnimator->Play(L"SquatStart");
	}

}

void PlayerStateScript::SquatMove()
{
}

void PlayerStateScript::Air()
{
}

void PlayerStateScript::Fall()
{
	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	if(rigidbody->GetVelocity().y==0)
	{
		mPlayer->SetPlayerState(Player::ePlayerState::Idle);
	}
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
	if (mAnimator->PlayAnimationName() != L"ThrowCap" )
	{
		mPlayer->GetMarioCap()->GetPhysical()->AddActorToPxScene();
		mAnimator->Play(L"ThrowCap", false);
	}

}

void PlayerStateScript::CatchCap()
{
	if (mAnimator->PlayAnimationName() != L"CatchCap")
	{
		mAnimator->Play(L"CatchCap", false);
	}
}

void PlayerStateScript::Die()
{
}
