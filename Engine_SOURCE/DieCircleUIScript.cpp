#include "DieCircleUIScript.h"
#include "TimeMgr.h"
#include "ImageUI.h"
#include "UIManager.h"
#include "CoinUIScript.h"

DieCircleUIScript::DieCircleUIScript()
	:UIScript()
	, mPanal(nullptr)
	, mTr(nullptr)
	, mState(State::None)
	, mCurrentTime(0.0f)
	, mOriginScale{}
{
}

DieCircleUIScript::~DieCircleUIScript()
{
}

void DieCircleUIScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner()->GetComponent<Transform>()->GetParent());
	mTr = this->GetOwner()->GetComponent<Transform>();
	mOriginScale = mTr->GetScale();
}

void DieCircleUIScript::Update()
{
	switch (mState)
	{
	case DieCircleUIScript::State::None:
		break;
	case DieCircleUIScript::State::Start:
		Start();
		break;
	case DieCircleUIScript::State::Idle:
		Idle();
		break;
	case DieCircleUIScript::State::ReStart:
		ReStart();
		break;
	case DieCircleUIScript::State::End:
		End();
		break;
	default:
		break;
	}
}

void DieCircleUIScript::Start()
{
	Vector3 scale = mTr->GetScale();

	scale.x -= DT * 30.f;
	scale.y -= DT * 30.f;

	mTr->SetScale(scale);


	if (scale.x < 4.f && scale.y < 4.f)
	{
		mState = State::Idle;
	}
}

void DieCircleUIScript::Idle()
{
	mCurrentTime += DT;

	if (mCurrentTime > 3.0f)
	{
		mState = State::ReStart;
		mCurrentTime = 0;
	}
}

void DieCircleUIScript::ReStart()
{
	Vector3 scale = mTr->GetScale();

	if (scale.x < 0.f && scale.y < 0.f)
	{
		scale = Vector3(0.f, 0.f, 1.0f);
		mTr->SetScale(scale);
		mState = State::End;

		Panal* panal = dynamic_cast<Panal*>((GETSINGLE(UIManager)->GetPanal(eUIType::Coin)));

		panal->Activate();
		panal->GetScript<CoinUIScript>()->ActionToPlayerDied();

		ImageUI* mUI = dynamic_cast<ImageUI*>(this->GetOwner());
		mUI->InActivate();
	}
	

	scale.x -= DT * 30.f;
	scale.y -= DT * 30.f;

	mTr->SetScale(scale);
}

void DieCircleUIScript::End()
{
	Vector3 scale = mTr->GetScale();

	scale.x += DT * 60.f;
	scale.y += DT * 60.f;

	mTr->SetScale(scale);


	if (scale.x > 20.f && scale.y > 20.f)
	{
		Reset();
	}
}


void DieCircleUIScript::Reset()
{
	mState = State::None;
	mCurrentTime = 0;
	mTr->SetScale(mOriginScale);

	(GETSINGLE(UIManager)->PlayerDieReset());
}
