#include "PlayerStateScript.h"
#include "Player.h"
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
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::Crouch, this));
	mStateEventList.emplace_back(std::bind(&PlayerStateScript::CrouchMove, this));
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
}

void PlayerStateScript::Move()
{
	BoneAnimator* animator = mPlayer->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	if (!rigidbody)
		return;

	PhysicalMovement* moveMent = GetOwner()->GetComponent<PhysicalMovement>();
	if (moveMent == nullptr)
		return;

	if (animator->PlayAnimationName() != L"Walk")
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
		mPlayer->SetPlayerState(Player::ePlayerState::Idle);
		return;
	}

	Transform* tr = mPlayer->GetComponent<Transform>();
	if (nullptr == tr)
		return;

	if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::UP))
	{
		if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::RIGHT))
		{
			tr->SetRotation(Vector3(0.0f, -135.f, 0.0f));
		}
		else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LEFT))
		{
			tr->SetRotation(Vector3(0.0f, -225.f, 0.0f));
		}
		else
		{
			tr->SetRotation(Vector3(0.0f, -180.f, 0.0f));
		}
	}
	else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::DOWN))
	{
		if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::RIGHT))
		{
			tr->SetRotation(Vector3(0.0f, -45.f, 0.0f));
		}
		else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LEFT))
		{
			tr->SetRotation(Vector3(0.0f, 45.f, 0.0f));
		}
		else
		{
			tr->SetRotation(Vector3(0.0f, 0.f, 0.0f));
		}

	}
	else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LEFT))
	{
		tr->SetRotation(Vector3(0.0f, 90.f, 0.0f));
	}
	else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::RIGHT))
	{
		tr->SetRotation(Vector3(0.0f, -90.f, 0.0f));
	}
	rigidbody->SetLinearMaxVelocityForDynamic(4000.f);
	rigidbody->AddForceForDynamic((-tr->Forward() * 9000.f * DT), PxForceMode::Enum::eFORCE);
}

void PlayerStateScript::Jump()
{
}

void PlayerStateScript::Crouch()
{
}

void PlayerStateScript::CrouchMove()
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
}

void PlayerStateScript::Die()
{
}
