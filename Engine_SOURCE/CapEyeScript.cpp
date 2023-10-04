#include "CapEyeScript.h"
#include "TimeMgr.h"
#include "ImageUI.h"
#include "Animator.h"

CapEyeScript::CapEyeScript()
	:UIScript()
	, mTr(nullptr)
	, mCurrentTime(0.0f)
	, mAnimationCnt(0)
	, mbEyeblink(false)
	, mbIdle(true)
{

}

CapEyeScript::~CapEyeScript()
{

}

void CapEyeScript::Initialize()
{
	mTr = this->GetOwner()->GetComponent<Transform>();
}

void CapEyeScript::Update()
{
	if (mbIdle)
	{
		mCurrentTime += DT;

		if (mCurrentTime >= 5)
		{
			Vector3 mScale = mTr->GetScale();
			mScale.x += DT * 12.f;
			mScale.y += DT * 18.f;

			mTr->SetScale(mScale);

			if (mScale.x > 1.0f && mScale.y > 1.5f)
			{
				mScale = Vector3(1.0f, 1.5f, 1.0f);
				mTr->SetScale(mScale);
				mCurrentTime = 0;
				mbIdle = false;
			}
		}
	}
	else
	{
		CapAnimation();
	}
}

void CapEyeScript::CapAnimation()
{
	mCurrentTime += DT;

	if (mCurrentTime >= 1.0f && mbEyeblink)
	{
		this->GetOwner()->GetComponent<Animator>()->Play(L"CapEyeAni", false);
		mCurrentTime = 0;
		mAnimationCnt++;
		mbEyeblink = false;
	}
	else if (!mbEyeblink)
	{
		if (mCurrentTime >= 2 && mAnimationCnt <= 4)
		{
			mbEyeblink = true;
			mCurrentTime = 0;
		}
		else if (mAnimationCnt >= 4)
		{
			Vector3 mScale = mTr->GetScale();
			mScale.x -= DT * 12.f;
			mScale.y -= DT * 18.f;

			mTr->SetScale(mScale);

			if (mScale.x < 0.f && mScale.y < 0.f)
			{
				mScale = Vector3(0.0f, 0.f, 1.0f);
				mTr->SetScale(mScale);
				mCurrentTime = 0;
				mAnimationCnt = 0;
				mbIdle = true;
				mbEyeblink = true;
			}
		}
	}
}
