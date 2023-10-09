#include "PlayerCameraScript.h"
#include "Transform.h"
#include "GameObj.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Camera.h"

#include "Scene.h"
#include "SceneMgr.h"
#include "SimpleMath.h"

#include "PhysXRayCast.h"


PlayerCameraScript::PlayerCameraScript()
	: mCameraObject(nullptr)
	, mTransform(nullptr)
	, mTarget(nullptr)
	, mSpeed(0.f)
	, mCamStep(0.f)
	, mLookAt(Vector3::Zero)
	, mbShaking(false)
	, mShakeParams{}
	, mShakeTimer(0.f)

{
}

PlayerCameraScript::~PlayerCameraScript()
{
}

void PlayerCameraScript::Initialize()
{
	mCameraObject = GetOwner()->GetComponent<Camera>();
	mTransform = GetOwner()->GetComponent<Transform>();
}

void PlayerCameraScript::Update()
{

	this->GetOwner();
	mLookAt = mTransform->GetPosition();

	mTarget = mCameraObject->GetTarget();

	mSpeed = mCameraObject->GetCamSpeed();
	mCamStep = 0.f;

	mCamStep = mSpeed * GETSINGLE(TimeMgr)->DeltaTimeConstant();

	KeyBoardMove();
	TargetMove();
	ShakeMove();

	mTransform->SetPosition(mLookAt);

	//if (mUICameraObject != nullptr)
	//{
	//	//mUICameraObject->GetComponent<Transform>()->SetPosition(mLookAt);
	//	//mUICameraObject->GetComponent<Transform>()->SetRotation(mTransform->GetRotation());z
	//}
}

void PlayerCameraScript::FixedUpdate()
{
}

void PlayerCameraScript::Render()
{
}

void PlayerCameraScript::OnCollisionEnter(Collider2D* _oppo)
{

}

void PlayerCameraScript::OnCollision(Collider2D* _oppo)
{

}

void PlayerCameraScript::OnCollisionExit(Collider2D* _oppo)
{

}

void PlayerCameraScript::KeyBoardMove()
{
	// Keyboard Move

	float speed = 50.f;
	if (KEY_DOWN(LSHIFT))
	{
		speed = 100.f;
	}

	if (KEY_DOWN(W))
	{
		mLookAt += speed * mTransform->Forward() * DT;
	}
	if (KEY_DOWN(S))
	{
		mLookAt -= speed * mTransform->Forward() * DT;
	}
	if (KEY_DOWN(A))
	{
		mLookAt -= speed * mTransform->Right() * DT;
	}
	if (KEY_DOWN(D))
	{
		mLookAt += speed * mTransform->Right() * DT;
	}
	if (KEY_DOWN(Q))
	{
		mLookAt -= 20.f * mTransform->Up() * DT;
	}
	if (KEY_DOWN(E))
	{
		mLookAt += 20.f * mTransform->Up() * DT;
	}
	//if (KEY_DOWN(Q))
	//{
	//	mLookAt -= 20.f * mPxTransform->Forward() * DT;
	//}
	//if (KEY_DOWN(E))
	//{
	//	mLookAt += 20.f * mPxTransform->Forward() * DT;
	//}

	if (KEY_DOWN(RBTN))
	{
		Vector3 rTapPos = GETSINGLE(InputMgr)->GetPrevMousePosition();
		Vector3 rCurPos = GETSINGLE(InputMgr)->GetMousePosition();
		float xAxisRotRatio = rCurPos.y - rTapPos.y;
		float yAxisRotRatio = rCurPos.x - rTapPos.x;

		mTransform->AddRotationX(xAxisRotRatio * -50.f * DT);
		mTransform->AddRotationY(yAxisRotRatio * 50.f * DT);
	}

}

void PlayerCameraScript::TargetMove()
{
	// Target Move
	if (mTarget)
	{
		if (mTarget->IsDead())
			mTarget = nullptr;
		else
		{
			if (mCamStep > mCameraObject->GetFarDistFromTarget())
			{
				mLookAt.x = mCameraObject->GetTarget()->GetPos().x;
				mLookAt.y = mCameraObject->GetTarget()->GetPos().y;

				mCameraObject->SetTarget(nullptr);
			}
			else
			{
				mLookAt += mCameraObject->GetCamDir() * mCamStep;
			}
		}
	}
}

void PlayerCameraScript::MakeCamShake(float _duration, float _Magnitude)
{
	ShakeParams sp = {};
	sp.duration = _duration;
	sp.magnitude = _Magnitude;
	Shake(sp);
}

void PlayerCameraScript::Shake(const ShakeParams& params)
{
	if (mbShaking)
	{
		// ���ο� ���� ȿ�� ����
		// ���� ��鸲 ȿ���� ���
		CancelShake();
	}

	mbShaking = true;
	mShakeParams = params;
	mShakeTimer = 0.f;
}

void PlayerCameraScript::CancelShake()
{
	mbShaking = false;
	mShakeParams = ShakeParams();
	mShakeTimer = 0.f;
}

void PlayerCameraScript::ShakeMove()
{
	if (mbShaking)
	{
		mShakeTimer += GETSINGLE(TimeMgr)->DeltaTimeConstant();
		if (mShakeTimer >= mShakeParams.duration)
		{
			// ��鸲 ���� �ð��� ������ ȿ�� ����
			CancelShake();
		}
		else {
			// ��鸲 ȿ�� ���
			float magnitude = mShakeParams.magnitude *
				(1.f - mShakeTimer / mShakeParams.duration);


			float randvalue = static_cast<float>(GetRandomNumber(1000) / 500.f) - 1.f;
			float x = randvalue * magnitude;
			randvalue = static_cast<float>(GetRandomNumber(1000) / 500.f) - 1.f;
			float y = randvalue * magnitude;


			mLookAt.x += x;
			mLookAt.y += y;
		}
	}
}

