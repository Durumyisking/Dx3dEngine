#include "CameraScript.h"
#include "Transform.h"
#include "GameObj.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Camera.h"

#include "Scene.h"
#include "SceneMgr.h"
#include "SimpleMath.h"

namespace dru
{
	CameraScript::CameraScript()
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

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
		mCameraObject = GetOwner()->GetComponent<Camera>();
		mTransform = GetOwner()->GetComponent<Transform>();

	}

	void CameraScript::update()
	{
		mLookAt = mTransform->GetPosition();

		mTarget = mCameraObject->GetTarget();
		mSpeed = mCameraObject->GetCamSpeed();
		mCamStep = 0.f;

		mCamStep = mSpeed * GETSINGLE(TimeMgr)->DeltaTimeConstant();
		
		KeyBoardMove();
		TargetMove();
		ShakeMove();

		mTransform->SetPosition(mLookAt);
	}

	void CameraScript::fixedUpdate()
	{
	}

	void CameraScript::render()
	{
	}


	void CameraScript::OnCollisionEnter(Collider2D* oppo)
	{

	}

	void CameraScript::OnCollision(Collider2D* oppo)
	{
		
	}

	void CameraScript::OnCollisionExit(Collider2D* oppo)
	{
		
	}

	void CameraScript::KeyBoardMove()
	{
		// Keyboard Move

		if (KEY_DOWN(U))
		{
			mLookAt += 10.f * mTransform->Up() * DT;
		}
		if (KEY_DOWN(J))
		{
			mLookAt += 10.f * -mTransform->Up() * DT;
		}
		if (KEY_DOWN(H))
		{
			mLookAt += 10.f * -mTransform->Right() * DT;
		}
		if (KEY_DOWN(K))
		{
			mLookAt += 10.f * mTransform->Right() * DT;
		}
		if (KEY_DOWN(P))
		{
			mLookAt -= 10.f * mTransform->Forward() * DT;
		}
		if (KEY_DOWN(O))
		{
			mLookAt += 10.f * mTransform->Forward() * DT;
		}

		if (KEY_DOWN(Q))
		{
			mTransform->AddRotationX(30.f * DT);
		}
		if (KEY_DOWN(W))
		{
			mTransform->AddRotationY(30.f * DT);
		}
		if (KEY_DOWN(E))
		{
			mTransform->AddRotationZ(30.f * DT);
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

	void CameraScript::MakeCamShake(float duration, float magnitude)
	{
		ShakeParams sp = {};
		sp.duration = duration;
		sp.magnitude = magnitude;
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



}