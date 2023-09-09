#include "GoombaStateScript.h"
#include "Monster.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"


GoombaStateScript::GoombaStateScript()
	: MonsterStateScript()
{
}

GoombaStateScript::~GoombaStateScript()
{
}

void GoombaStateScript::Update()
{
	MonsterStateScript::Update();
}

void GoombaStateScript::Idle()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	if (animator->PlayAnimationName() != L"Wait")
	{
		animator->Play(L"Wait");
	}
}

void GoombaStateScript::Move()
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

	if (animator->PlayAnimationName() != L"Dash")
	{
		const std::wstring& test = animator->PlayAnimationName();
		animator->Play(L"Dash");
	}

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
	rigidbody->SetLinearMaxVelocityForDynamic(4000.f);
	rigidbody->AddForceForDynamic((-tr->Forward() * 100000.f * DT), PxForceMode::Enum::eFORCE);
}

void GoombaStateScript::Jump()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	PhysXRigidBody* rigidbody = mMonster->GetComponent<PhysXRigidBody>();
	if (!rigidbody)
		return;

	PhysicalMovement* moveMent = mMonster->GetComponent<PhysicalMovement>();
	if (!moveMent)
		return;


	if (animator->PlayAnimationName() != L"Jump")
	{
		const std::wstring& test = animator->PlayAnimationName();
		animator->Play(L"Jump", false);

		rigidbody->SetLinearMaxVelocityForDynamic(5000.f);
		rigidbody->AddForce(math::Vector3(0.0f, 100000.f / 5.0f, 0.0f), physx::PxForceMode::eFORCE);
		rigidbody->SetLinearDamping(1.0f);
	}

	if (animator->PlayAnimationName() == L"Jump" && animator->IsCompleate())
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		rigidbody->SetLinearMaxVelocityForDynamic(5000.f);
	}
}

void GoombaStateScript::SpecialSituation()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	std::wstring testName = L"Turn";

	if (animator->PlayAnimationName() != testName)
	{
		animator->Play(testName);
	}
}

void GoombaStateScript::Hit()
{
}

void GoombaStateScript::Groggy()
{
}

void GoombaStateScript::Die()
{
}
