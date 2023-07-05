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

		mTarget = mCameraObject->mTargetObj;
		mSpeed = mCameraObject->mCamSpeed;
		mCamStep = 0.f;

		mCamStep = mSpeed * TimeMgr::DeltaTimeConstant();
		
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

		if (Input::GetKeyState(eKeyCode::U) == eKeyState::DOWN)
		{
			mLookAt += 10.f * mTransform->Up() * TimeMgr::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::J) == eKeyState::DOWN)
		{
			mLookAt += 10.f * -mTransform->Up() * TimeMgr::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::H) == eKeyState::DOWN)
		{
			mLookAt += 10.f * -mTransform->Right() * TimeMgr::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::K) == eKeyState::DOWN)
		{
			mLookAt += 10.f * mTransform->Right() * TimeMgr::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::P) == eKeyState::DOWN)
		{
			mLookAt -= 10.f * mTransform->Forward() * TimeMgr::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::O) == eKeyState::DOWN)
		{
			mLookAt += 10.f * mTransform->Forward() * TimeMgr::DeltaTime();
		}

		if (Input::GetKeyDown(eKeyCode::Q))
		{
			mTransform->AddRotationX(30.f * TimeMgr::DeltaTime());
		}
		if (Input::GetKeyDown(eKeyCode::W))
		{
			mTransform->AddRotationY(30.f * TimeMgr::DeltaTime());
		}
		if (Input::GetKeyDown(eKeyCode::E))
		{
			mTransform->AddRotationZ(30.f * TimeMgr::DeltaTime());
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

	void CameraScript::MakeCamShake(float _duration, float _Magnitude)
	{
		ShakeParams sp = {};
		sp.duration = _duration;
		sp.magnitude = _Magnitude;
		renderer::mainCamera->GetCamScript()->Shake(sp);
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
			mShakeTimer += TimeMgr::DeltaTimeConstant();
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