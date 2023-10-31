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
#include "MeshRenderer.h"


GoombaStateScript::GoombaStateScript()
	: MonsterStateScript()
	, mAnimator (nullptr)
	, mRigidbody(nullptr)
	, mMovement (nullptr)
	, mTransform(nullptr)
	, mModel(nullptr)
{
}

GoombaStateScript::~GoombaStateScript()
{
}

void GoombaStateScript::Initialize()
{
	MonsterStateScript::Initialize();
	mAnimator = mMonster->GetComponent<BoneAnimator>();

	mAnimator->GetAnimationClip(L"Land")->SetCompleteEvent(
		[this]()
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
	}
	);
	mAnimator->GetAnimationClip(L"Jump")->SetCompleteEvent(
		[this]()
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Fall);
	}
	);
	mAnimator->GetAnimationClip(L"Find")->SetCompleteEvent(
		[this]()
	{
		mRigidbody->SetMaxVelocity_XZ(GOOMBA_RUN_VELOCITY);
		mAnimator->Play(L"Dash");
	}
	);
	mAnimator->GetAnimationClip(L"PressDown")->SetCompleteEvent(
		[this]()
	{
		mMonster->Die();
	}
	);


	mRigidbody = mMonster->GetComponent<PhysXRigidBody>();
	mRigidbody->SetMaxVelocity_XZ(GOOMBA_WALK_VELOCITY);
	mRigidbody->SetMaxVelocity_Y(15.f);
	mMovement = mMonster->GetComponent<PhysicalMovement>();
	mTransform = mMonster->GetComponent<Transform>();

	mModel = GetMeshRenderer()->GetModel();
}

void GoombaStateScript::Update()
{
	if (KEY_TAP(N_6))
	{
		if (!mRigidbody->IsOnAir())
		{
			mRigidbody->SetAirOn();
			mRigidbody->RemoveGravity();
		}
		else
		{
			mRigidbody->SetAirOff();
			mRigidbody->ApplyGravity();

		}
	}
	MonsterStateScript::Update();
}

void GoombaStateScript::Idle()
{
	if (mAnimator->PlayAnimationName() != L"Wait")
	{
		//setOpenEyeModel();
		mAnimator->Play(L"Wait");
	}

	MonsterStateScript::Idle();
}

void GoombaStateScript::Move()
{
	rotateByKey();
	if (mMonster->IsCapture())
	{
		if (!mRigidbody->IsOnAir())
		{
			if (KEY_DOWN(LSHIFT))
			{
				if (mAnimator->PlayAnimationName() != L"Run")
				{
					mRigidbody->SetMaxVelocity_XZ(GOOMBA_RUN_VELOCITY);
					mAnimator->Play(L"Run");
				}
			}
			if (KEY_NONE(LSHIFT))
			{
				if (mAnimator->PlayAnimationName() != L"Walk")
				{
					mRigidbody->SetMaxVelocity_XZ(GOOMBA_WALK_VELOCITY);
					mAnimator->Play(L"Walk");
				}
			}
			if (KEY_TAP(SPACE))
			{
				Jump();
				return;
			}
		}
	}
	else
	{
		if (mAnimator->PlayAnimationName() != L"Walk")
		{
			mRigidbody->SetMaxVelocity_XZ(GOOMBA_WALK_VELOCITY);
			mAnimator->Play(L"Walk");
		}
	}

	if (!KEY_NONE(W) || !KEY_NONE(S) || !KEY_NONE(A) || !KEY_NONE(D))
	{
		MoveForward(GOOMBA_SPPED);
	}
}

void GoombaStateScript::Jump()
{
	if (!mRigidbody->IsOnAir())
	{
		mAnimator->Play(L"Jump", false);
		mRigidbody->AddForce(math::Vector3(0.0f, GOOMBA_JUMPFORCE, 0.0f));
		mRigidbody->ApplyGravity();
		mRigidbody->SetAirOn();
	}
}

void GoombaStateScript::Fall()
{
	if (mAnimator->PlayAnimationName() != L"Fall")
	{
		mAnimator->Play(L"Fall");
	}
}

void GoombaStateScript::Land()
{
	if (mAnimator->PlayAnimationName() != L"Land")
	{
		mAnimator->Play(L"Land", false);
	}
}

void GoombaStateScript::Turn()
{
	Vector3 dirToPlayer_XYZ = GetTransform()->GetWorldPosition() - mPlayer->GetWorldPos();
	Vector2 dirToPlayer = { dirToPlayer_XYZ.x, dirToPlayer_XYZ.z };
	Vector2 worldForward = { GetTransform()->WorldForward().x, GetTransform()->WorldForward().z };
	dirToPlayer.Normalize();
	worldForward.Normalize();
	float rotCosTheta = dirToPlayer.Dot(worldForward);
	if (rotCosTheta < 0.99f)
	{		
		if (mAnimator->PlayAnimationName() != L"Turn")
		{
			mAnimator->Play(L"Turn");
		}
		if (mbTurnLeft)
		{
			mTransform->AddPhysicalRotation(Vector3(0.f, -360.f, 0.f) * DT);
		}
		else
		{
			mTransform->AddPhysicalRotation(Vector3(0.f, 360.f, 0.f) * DT);
		}
	}
	else
	{		
		mTransform->AddPhysicalRotation_Radian(Vector3(0.f, rotCosTheta, 0.f) * DT);
		if (mAnimator->PlayAnimationName() != L"Find")
		{
			mMonster->SetMonsterState(Monster::eMonsterState::Chase);
			mAnimator->Play(L"Find", false);
		}
	}
}

void GoombaStateScript::Chase()
{
	if (mAnimator->PlayAnimationName() == L"Dash")
	{
		Vector3 dirToPlayer = mPlayer->GetWorldPos() - GetOwnerWorldPos();
		dirToPlayer.Normalize();
		float rotCosTheta = dirToPlayer.Dot(mTransform->WorldForward());
		//mTransform->AddPhysicalRotation_Radian(Vector3(0.f, rotCosTheta, 0.f) * DT);

		Vector3 moveDir = -mTransform->WorldForward();
		moveDir.y = 0.f;
		mRigidbody->AddForce((moveDir * GOOMBA_SPPED * DT));
	}
}

void GoombaStateScript::SpecialSituation()
{
	std::wstring testName = L"Turn";

	if (mAnimator->PlayAnimationName() != testName)
	{
		mAnimator->Play(testName);
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

	if (mAnimator->GetAnimationClip(L"PressDown")->IsComplete())
	{
		mMonster->Die();
		// 죽는 이펙트 재생시켜야한다.
	}
}

void GoombaStateScript::rotateByKey()
{
	if (!mRigidbody->IsOnAir())
	{
		if (GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::W)
			&& GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::S)
			&& GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::A)
			&& GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::D))
		{
			mMonster->SetMonsterState(Monster::eMonsterState::Idle);
			return;
		}
	}

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
				mTransform->SetPhysicalRotation(rotation);
				able = true;
			}
		}
	};

	Input_DownFunC(eKeyCode::W, eKeyCode::D, math::Vector3(0.0f, 225.f, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::A, math::Vector3(0.0f, 135.f, 0.0f));
	Input_DownFunC(eKeyCode::W, eKeyCode::W, math::Vector3(0.0f, 180.f, 0.0f));

	Input_DownFunC(eKeyCode::S, eKeyCode::D, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::A, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::S, eKeyCode::S, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::A, eKeyCode::A, math::Vector3(0.0f, 90.f, 0.0f));
	Input_DownFunC(eKeyCode::D, eKeyCode::D, math::Vector3(0.0f, -90.f, 0.0f));
}
