#include "GoombaStateScript.h"
#include "Monster.h"
#include "InputMgr.h"


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
	}
}

void GoombaStateScript::Jump()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	if (animator->PlayAnimationName() != L"Jump")
	{
		const std::wstring& test = animator->PlayAnimationName();
		animator->Play(L"Jump", false);
	}

	if (animator->PlayAnimationName() == L"Jump" && animator->IsCompleate())
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
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
