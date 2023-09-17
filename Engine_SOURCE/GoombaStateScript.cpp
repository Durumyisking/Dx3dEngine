#include "GoombaStateScript.h"
#include "Monster.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"

#include "PhysXCollider.h"

#include "TimerMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

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
	mRigidbody->SetLinearMaxVelocityForDynamic(GOOMBA_RUN_VELOCITY);
	mRigidbody->SetMassForDynamic(GOOMBA_MASS);
	mRigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	mRigidbody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);
	mMovement = mMonster->GetComponent<PhysicalMovement>();
	mTransform = mMonster->GetComponent<Transform>();

	Timer* antiGlideTimer = new Timer(0.1f);
	antiGlideTimer->SetDestroy(false);
	antiGlideTimer->Event() = [this]
	{
		mRigidbody->AddForceForDynamic((Vector3(0.f, -1.f, 0.f) * GOOMBA_SPPED * DT), PxForceMode::Enum::eIMPULSE);
	};
	GETSINGLE(TimerMgr)->GetInstance()->AddTimer(antiGlideTimer);
}

void GoombaStateScript::Update()
{
	if (KEY_TAP(N_3))
	{
		mAnimator->Play(L"Find");
	}
	if (KEY_TAP(N_4))
	{
		mAnimator->Play(L"Turn");
	}
	if (KEY_TAP(N_5))
	{
		mAnimator->Play(L"Land");
	}
	if (KEY_TAP(N_6))
	{
		mAnimator->Play(L"Fall");
	}
	if (KEY_TAP(N_7))
	{
		mAnimator->Play(L"Slide");
	}

	float dist = Vector3::Distance(GETSINGLE(SceneMgr)->GetActiveScene()->GetPlayer()->GetWorldPos(), GetOwnerWorldPos());
	if (10.f > dist)
	{

	}
	else
	{

	}


	MonsterStateScript::Update();
}

void GoombaStateScript::Idle()
{
	if (mAnimator->PlayAnimationName() != L"Wait")
	{
		mAnimator->Play(L"Wait");
	}

}

void GoombaStateScript::Move()
{
	if (mAnimator->PlayAnimationName() != L"Walk")
	{
		const std::wstring& anim = mAnimator->PlayAnimationName();
		mRigidbody->SetLinearMaxVelocityForDynamic(GOOMBA_WALK_VELOCITY);
		mAnimator->Play(L"Walk");
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


	Vector3 moveDir = mTransform->Forward();
	moveDir.y = 0.f;
	mRigidbody->AddForceForDynamic((-moveDir * GOOMBA_SPPED * DT), PxForceMode::Enum::eIMPULSE);
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
