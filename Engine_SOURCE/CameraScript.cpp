#include "CameraScript.h"
#include "Transform.h"
#include "GameObj.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Camera.h"

#include "Scene.h"
#include "SceneMgr.h"
#include "SimpleMath.h"

#include "PhysXRayCast.h"


CameraScript::CameraScript()
	: mCameraObject(nullptr)
	, mUICameraObject(nullptr)
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

CameraScript::~CameraScript()
{
}

void CameraScript::Initialize()
{
	mCameraObject = GetOwner()->GetComponent<Camera>();
	mTransform = GetOwner()->GetComponent<Transform>();
}

void CameraScript::Update()
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

	if (mUICameraObject != nullptr)
	{

		mUICameraObject->GetComponent<Transform>()->SetPosition(mLookAt);
		//mUICameraObject->GetComponent<Transform>()->SetRotation(mTransform->GetRotation());
	}
}

void CameraScript::FixedUpdate()
{
}

void CameraScript::Render()
{
}

void CameraScript::OnCollisionEnter(Collider2D* _oppo)
{

}

void CameraScript::OnCollision(Collider2D* _oppo)
{
		
}

void CameraScript::OnCollisionExit(Collider2D* _oppo)
{
		
}

void CameraScript::KeyBoardMove()
{
	// Keyboard Move

	float speed = 100.f;
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

void CameraScript::TargetMove()
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

void CameraScript::MakeCamShake(float _duration, float _Magnitude)
{
	ShakeParams sp = {};
	sp.duration = _duration;
	sp.magnitude = _Magnitude;
	Shake(sp);
}

void CameraScript::Shake(const ShakeParams& params)
{
	if (mbShaking) 
	{
		// 새로운 흔들기 효과 들어가면
		// 이전 흔들림 효과를 취소
		CancelShake();
	}

	mbShaking = true;
	mShakeParams = params;
	mShakeTimer= 0.f;
}

void CameraScript::CancelShake()
{
	mbShaking = false;
	mShakeParams = ShakeParams();
	mShakeTimer = 0.f;
}

void CameraScript::ShakeMove()
{
	if (mbShaking)
	{
		mShakeTimer += GETSINGLE(TimeMgr)->DeltaTimeConstant();
		if (mShakeTimer >= mShakeParams.duration)
		{
			// 흔들림 지속 시간이 지나면 효과 종료
			CancelShake();
		}
		else {				
			// 흔들림 효과 계산
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

