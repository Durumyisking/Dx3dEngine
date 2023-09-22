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
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Wall, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Hit, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Groggy, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::ThrowCap, this));
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
}

void PlayerStateScript::Idle()
{
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;
	if(!(animator->PlayAnimationName()== L"Wait"))
		animator->Play(L"Wait");
	//mInitialForce = 33.f;
}

void PlayerStateScript::Move()
{
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	assert(animator);

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	assert(rigidbody);
		
	PhysicalMovement* moveMent = GetOwner()->GetComponent<PhysicalMovement>();
	assert(moveMent);

	Physical* physical = GetOwner()->GetComponent<Physical>();
	assert(physical);

	if (animator->PlayAnimationName() == L"Brake")
		return;

	if (animator->PlayAnimationName() != L"Walk" 
		&& animator->PlayAnimationName() != L"Run"
		&& animator->PlayAnimationName() != L"RunStart")
	{
		const std::wstring& test = animator->PlayAnimationName();
		animator->Play(L"Walk");
		//mPlayer->PlayerAnimation(L"Walk");
	}

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
			animator->Play(L"Brake");
			//mPlayer->SetPlayerState(Player::ePlayerState::Idle);
			//mMoveTime = 0.0f;
			//rigidbody->SetLinearMaxVelocityForDynamic(5.f);
			//mInitialForce = 33.f;
			return;
		}
	}

	Transform* tr = mPlayer->GetComponent<Transform>();
	if (nullptr == tr)
		return;


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

	//rigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	//rigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);
	rigidbody->SetMaxVelocity(10.f);


	// 매 프레임마다 힘을 증가시킴
	//if (mInitialForce < 40.f)
	//mMoveTime += DT;
	//mInitialForce += mForceIncrement;
	/*if (mMoveTime > 1.f)
	{
		if (animator->PlayAnimationName() != L"Run")
			animator->Play(L"Run");
	}*/
	//rigidbody->AddForceForDynamic((-tr->Forward() * mInitialForce * DT), PxForceMode::Enum::eIMPULSE);

	rigidbody->AddForce((-tr->Forward() * 10000.f));

	
	//std::cout << rigidbody->GetForce().x << rigidbody->GetForce().y << rigidbody->GetForce().z << std::endl;
}

void PlayerStateScript::Jump()
{
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	if (!rigidbody)
		return;

	if (animator->PlayAnimationName() != L"Jump")
	{
		const std::wstring& test = animator->PlayAnimationName();
		animator->Play(L"Jump", false);

		rigidbody->SetLinearMaxVelocityForDynamic(1000.f);
		//rigidbody->AddForce(math::Vector3(0.0f, 5000.f, 0.0f), physx::PxForceMode::eFORCE);
		rigidbody->SetLinearDamping(1.0f);
	}

	if (animator->PlayAnimationName() == L"Jump" && animator->IsComplete())
	{
		mPlayer->SetPlayerState(Player::ePlayerState::Idle);
		rigidbody->SetLinearMaxVelocityForDynamic(100.f);
	}

}

void PlayerStateScript::Squat()
{
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	if (!rigidbody)
		return;

	if (animator->PlayAnimationName() != L"SquatStart" 
		&& animator->PlayAnimationName() != L"SquatMove"
		&& animator->PlayAnimationName() != L"SquatWait")
	{
		const std::wstring& test = animator->PlayAnimationName();
		animator->Play(L"SquatStart");
	}

}

void PlayerStateScript::SquatMove()
{
}

void PlayerStateScript::Air()
{
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
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	if (animator->PlayAnimationName() != L"ThrowCap" )
	{
		animator->Play(L"ThrowCap",false);
	}

}

void PlayerStateScript::Die()
{
}
