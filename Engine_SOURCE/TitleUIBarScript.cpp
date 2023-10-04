#include "TitleUIBarScript.h"
#include "ImageUI.h"
#include "TimeMgr.h"
#include "InputMgr.h"

TitleUIBarScript::TitleUIBarScript()
	:UIScript()
	, mCurrentTime(0.0f)
	, mState(State::Idle)
	, mOriginScale{}
	, mChangeSize(Vector3(1.f, 0.9f, 1.0f))
{

}

TitleUIBarScript::~TitleUIBarScript()
{

}

void TitleUIBarScript::Initialize()
{
	mOriginScale = this->GetOwner()->GetComponent<Transform>()->GetScale();
}

void TitleUIBarScript::Update()
{
	Vector3 scale = this->GetOwner()->GetComponent<Transform>()->GetScale();
	scale.x -= DT;
	this->GetOwner()->GetComponent<Transform>()->SetScaleX(scale.x);


	switch (mState)
	{
	case TitleUIBarScript::State::Idle:
		Idle();
		break;
	case TitleUIBarScript::State::ScaleChange:
		ScaleChange();
		break;
	default:
		break;
	}
}

void TitleUIBarScript::Idle()
{
	if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::UP)))
	{
		Vector3 pos = this->GetOwner()->GetComponent<Transform>()->GetPosition() + Vector3(0.0f, 1.f, 0.0f);
		this->GetOwner()->GetComponent<Transform>()->SetPosition(pos);

		pos = mOriginScale * mChangeSize;  //Vector3(1.f, 0.9f, 1.0f)
		this->GetOwner()->GetComponent<Transform>()->SetScale(pos);
		mState = State::ScaleChange;
	}
	else if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::DOWN)))
	{
		Vector3 pos = this->GetOwner()->GetComponent<Transform>()->GetPosition() + Vector3(0.0f, -1.f, 0.0f);
		this->GetOwner()->GetComponent<Transform>()->SetPosition(pos);

		pos = mOriginScale * mChangeSize;
		this->GetOwner()->GetComponent<Transform>()->SetScale(pos);
		mState = State::ScaleChange;
	}
}

void TitleUIBarScript::ScaleChange()
{
	Vector3 scale = this->GetOwner()->GetComponent<Transform>()->GetScale() + Vector3(DT, DT, 0.0f);

	this->GetOwner()->GetComponent<Transform>()->SetScale(scale);

	if (scale.x > mOriginScale.x && scale.y > mOriginScale.y)
	{
		this->GetOwner()->GetComponent<Transform>()->SetScale(mOriginScale);
		mCurrentTime = 0;
		mState = State::Idle;
	}
}

void TitleUIBarScript::Select()
{

}
