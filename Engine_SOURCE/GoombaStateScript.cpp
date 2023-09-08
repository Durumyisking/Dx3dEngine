#include "GoombaStateScript.h"
#include "Monster.h"
#include "InputMgr.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"


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
		const std::wstring& test = animator->PlayAnimationName();
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

	if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::UP))
	{

	}
	else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::DOWN))
	{

	}
	else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LEFT))
	{

	}
	else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::RIGHT))
	{

	}
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


		//moveMent->Move(math::Vector3(0.0f, 10000.f, 0.0f));
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

void GoombaStateScript::SpecialCast()
{
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
