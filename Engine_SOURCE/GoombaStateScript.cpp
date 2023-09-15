#include "GoombaStateScript.h"
#include "Monster.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"


GoombaStateScript::GoombaStateScript()
	: MonsterStateScript()
	, mAnimator (nullptr)
	, mRigidbody(nullptr)
	, mMovement (nullptr)
	, mTransform(nullptr)
{
}

GoombaStateScript::~GoombaStateScript()
{
}

void GoombaStateScript::Initialize()
{
	MonsterStateScript::Initialize();
	mAnimator = mMonster->GetComponent<BoneAnimator>();
	mRigidbody = mMonster->GetComponent<PhysXRigidBody>();
	mRigidbody->SetLinearMaxVelocityForDynamic(100.f);
	mMovement = mMonster->GetComponent<PhysicalMovement>();
	mTransform = mMonster->GetComponent<Transform>();
}

void GoombaStateScript::Update()
{
	MonsterStateScript::Update();
}

void GoombaStateScript::Idle()
{
	if (mAnimator->PlayAnimationName() != L"Wait")
	{
		mAnimator->Play(L"Wait");
	}

	mRigidbody->AddForceForDynamic((Vector3(0.f, -1.f, 0.f) * 100.f * DT), PxForceMode::Enum::eIMPULSE);

}

void GoombaStateScript::Move()
{
	if (mAnimator->PlayAnimationName() != L"Dash")
	{
		const std::wstring& test = mAnimator->PlayAnimationName();
		mAnimator->Play(L"Dash");
	}

	if (GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::UP) || GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::DOWN)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::LEFT) || GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::RIGHT))
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		return;
	}

	bool able = false;
	auto Input_DownFunC = [&](eKeyCode key, eKeyCode mult_key, math::Vector3 rotation)
		{
			if (able)
				return;

			if (GETSINGLE(InputMgr)->GetKeyDown(key))
			{
				if (GETSINGLE(InputMgr)->GetKeyDown(mult_key))
				{
					mTransform->SetPhysicalRotation(rotation);
					able = true;
				}
			}
		};

	Input_DownFunC(eKeyCode::UP, eKeyCode::RIGHT, math::Vector3(0.0f, -135.f, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::LEFT, math::Vector3(0.0f, -225.f, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::UP, math::Vector3(0.0f, -180.f, 0.0f));

	Input_DownFunC(eKeyCode::DOWN, eKeyCode::RIGHT, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::LEFT, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::DOWN, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::LEFT, eKeyCode::LEFT, math::Vector3(0.0f, 90.f, 0.0f));
	Input_DownFunC(eKeyCode::RIGHT, eKeyCode::RIGHT, math::Vector3(0.0f, -90.f, 0.0f));


	mRigidbody->AddForceForDynamic((-mTransform->Forward() * 100.f * DT), PxForceMode::Enum::eIMPULSE);
}

void GoombaStateScript::Jump()
{
	if (mAnimator->PlayAnimationName() != L"Jump")
	{
		const std::wstring& test = mAnimator->PlayAnimationName();
		mAnimator->Play(L"Jump", false);

		mRigidbody->SetLinearMaxVelocityForDynamic(1000.f);
		mRigidbody->AddForce(math::Vector3(0.0f, 5000.f, 0.0f), physx::PxForceMode::eFORCE);
		mRigidbody->SetLinearDamping(1.0f);
	}

	if (mAnimator->PlayAnimationName() == L"Jump" && mAnimator->IsCompleate())
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		mRigidbody->SetLinearMaxVelocityForDynamic(100.f);
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
