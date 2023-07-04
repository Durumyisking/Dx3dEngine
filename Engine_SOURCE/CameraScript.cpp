#include "CameraScript.h"
#include "Transform.h"
#include "GameObj.h"
#include "Input.h"
#include "TimeMgr.h"
#include "Camera.h"

#include "Scene.h"
#include "SceneMgr.h"

namespace dru
{
	CCameraScript::CCameraScript()
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

	CCameraScript::~CCameraScript()
	{
	}

	void CCameraScript::Initialize()
	{
		mCameraObject = GetOwner()->GetComponent<CCamera>();
		mTransform = GetOwner()->GetComponent<CTransform>();

	}

	void CCameraScript::update()
	{
		mLookAt = mTransform->GetPosition();

		mTarget = mCameraObject->mTargetObj;
		mSpeed = mCameraObject->mCamSpeed;
		mCamStep = 0.f;

		mCamStep = mSpeed * CTimeMgr::DeltaTimeConstant();
		
		KeyBoardMove();
		TargetMove();
		ShakeMove();

		mTransform->SetPosition(mLookAt);
	}

	void CCameraScript::fixedUpdate()
	{
	}

	void CCameraScript::render()
	{
	}

	void CCameraScript::OnCollisionEnter(CCollider2D* _oppo)
	{

	}

	void CCameraScript::OnCollision(CCollider2D* _oppo)
	{
		
	}

	void CCameraScript::OnCollisionExit(CCollider2D* _oppo)
	{
		
	}

	void CCameraScript::KeyBoardMove()
	{
		// Keyboard Move

		if (CInput::GetKeyState(eKeyCode::U) == eKeyState::DOWN)
		{
			mLookAt += 10.f * mTransform->Up() * CTimeMgr::DeltaTime();
		}
		if (CInput::GetKeyState(eKeyCode::J) == eKeyState::DOWN)
		{
			mLookAt += 10.f * -mTransform->Up() * CTimeMgr::DeltaTime();
		}
		if (CInput::GetKeyState(eKeyCode::H) == eKeyState::DOWN)
		{
			mLookAt += 10.f * -mTransform->Right() * CTimeMgr::DeltaTime();
		}
		if (CInput::GetKeyState(eKeyCode::K) == eKeyState::DOWN)
		{
			mLookAt += 10.f * mTransform->Right() * CTimeMgr::DeltaTime();
		}
		if (CInput::GetKeyState(eKeyCode::P) == eKeyState::DOWN)
		{
			mLookAt -= 10.f * mTransform->Forward() * CTimeMgr::DeltaTime();
		}
		if (CInput::GetKeyState(eKeyCode::O) == eKeyState::DOWN)
		{
			mLookAt += 10.f * mTransform->Forward() * CTimeMgr::DeltaTime();
		}

		if (CInput::GetKeyDown(eKeyCode::Q))
		{
			mTransform->AddRotationX(20.f * CTimeMgr::DeltaTime());
		}
		if (CInput::GetKeyDown(eKeyCode::W))
		{
			mTransform->AddRotationY(20.f * CTimeMgr::DeltaTime());
		}
		if (CInput::GetKeyDown(eKeyCode::E))
		{
			mTransform->AddRotationZ(20.f * CTimeMgr::DeltaTime());
		}

	}

	void CCameraScript::TargetMove()
	{
		// Target Move
		if (mTarget)
		{
			if (mTarget->IsDead())
				mTarget = nullptr;
			else
			{
				if (mCamStep > mCameraObject->mFarDist)
				{
					mLookAt.x = mCameraObject->mTargetObj->GetPos().x;
					mLookAt.y = mCameraObject->mTargetObj->GetPos().y;

					mCameraObject->mTargetObj = nullptr;
				}
				else
				{
					mLookAt += mCameraObject->mCamDir * mCamStep;
				}
			}
		}
	}

	void CCameraScript::MakeCamShake(float _duration, float _Magnitude)
	{
		ShakeParams sp = {};
		sp.duration = _duration;
		sp.magnitude = _Magnitude;
		renderer::mainCamera->GetCamScript()->Shake(sp);
	}

	void CCameraScript::Shake(const ShakeParams& params)
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

	void CCameraScript::CancelShake()
	{
		mbShaking = false;
		mShakeParams = ShakeParams();
		mShakeTimer = 0.f;
	}

	void CCameraScript::ShakeMove()
	{
		if (mbShaking)
		{
			mShakeTimer += CTimeMgr::DeltaTimeConstant();
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