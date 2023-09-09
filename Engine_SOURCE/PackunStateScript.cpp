#include "PackunStateScript.h"
#include "Monster.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"

PackunStateScript::PackunStateScript()
	:MonsterStateScript()
{
}

PackunStateScript::~PackunStateScript()
{
}

void PackunStateScript::Update()
{
	MonsterStateScript::Update();
}

void PackunStateScript::Idle()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	if (animator->PlayAnimationName() != L"Wait")
	{
		animator->Play(L"Wait");
	}
}

void PackunStateScript::Move()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	if (!rigidbody)
		return;

	PhysicalMovement* moveMent = GetOwner()->GetComponent<PhysicalMovement>();
	if (moveMent == nullptr)
		return;

	if (GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::UP)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::DOWN)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::LEFT)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::RIGHT))
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		return;
	}

	Transform* tr = mMonster->GetComponent<Transform>();
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
}

void PackunStateScript::Attack()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	std::wstring animationName = L"Attack";
	if (animator->PlayAnimationName() != animationName)
	{
		animator->Play(animationName);
	}
}

void PackunStateScript::SpecialCast()
{
}

void PackunStateScript::Hit()
{
}

void PackunStateScript::Groggy()
{
}

void PackunStateScript::Die()
{
}
