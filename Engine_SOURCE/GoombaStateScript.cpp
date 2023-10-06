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
		//setHalfCloseEyeModel();
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
	if (mMonster->IsCapture())
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
	}
	else
	{
		if (mAnimator->PlayAnimationName() != L"Walk")
		{
			mRigidbody->SetMaxVelocity_XZ(GOOMBA_WALK_VELOCITY);
			mAnimator->Play(L"Walk");
		}
	}

	rotateByKey();
	
	Vector3 moveDir = -mTransform->WorldForward();
	moveDir.y = 0.f;
	mRigidbody->AddForce((moveDir * GOOMBA_SPPED * DT));
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
		//mRigidbody->AddForce((moveDir * GOOMBA_SPPED * DT));
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
		// �״� ����Ʈ ������Ѿ��Ѵ�.
	}
}

void GoombaStateScript::setHalfCloseEyeModel()
{
	assert(mModel);

	mModel->MeshRenderSwtich(L"EyeOpen__BodyMT-mesh", false);
	mModel->MeshRenderSwtich(L"EyeOpen__EyeLMT-mesh", false);
	mModel->MeshRenderSwtich(L"EyeOpen__EyeRMT-mesh", false);

	mModel->MeshRenderSwtich(L"EyeHalfClose__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeLMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeRMT-mesh", true);
}

void GoombaStateScript::setOpenEyeModel()
{
	assert(mModel);

	mModel->MeshRenderSwtich(L"EyeOpen__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeOpen__EyeLMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeOpen__EyeRMT-mesh", true);

	mModel->MeshRenderSwtich(L"EyeHalfClose__BodyMT-mesh", false);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeLMT-mesh", false);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeRMT-mesh", false);
}

void GoombaStateScript::rotateByKey()
{
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

	Input_DownFunC(eKeyCode::UP, eKeyCode::RIGHT, math::Vector3(0.0f, 225.f, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::LEFT, math::Vector3(0.0f, 135.f, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::UP, math::Vector3(0.0f, 180.f, 0.0f));

	Input_DownFunC(eKeyCode::DOWN, eKeyCode::RIGHT, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::LEFT, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::DOWN, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::LEFT, eKeyCode::LEFT, math::Vector3(0.0f, 90.f, 0.0f));
	Input_DownFunC(eKeyCode::RIGHT, eKeyCode::RIGHT, math::Vector3(0.0f, -90.f, 0.0f));
}
