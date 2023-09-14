#include "HUD.h"
#include "TimeMgr.h"
#include "InputMgr.h"

HUD::HUD()
	:UIBase(eUIType::HP)
	, mSpeed(0)
	, mCurrentTime(0)
	, mChangeSize(Vector3::One)
	, mTargetPos(Vector3::Zero)
	, mState(HUDState::None)
	, mActivate(false)
	, mbGoAndReturn(false)
	, mCount(3)
{

}

HUD::HUD(eUIType type)
	:UIBase(type)
	, mSpeed(0)
	, mCurrentTime(0)
	, mChangeSize(Vector3::One)
	, mTargetPos(Vector3::Zero)
	, mState(HUDState::None)
	, mActivate(false)
	, mbGoAndReturn(false)
	, mCount(3)
{

}

HUD::~HUD()
{

}

void HUD::OnInit()
{
	mOriginPos = this->GetComponent<Transform>()->GetPosition();
	mOriginScale = this->GetComponent<Transform>()->GetScale();
}

void HUD::OnActive()
{
	mActivate = true;
	this->Active();
}

void HUD::OnInActive()
{
	mActivate = false;
	this->Die();
}

void HUD::OnUpdate()
{
	if (mActivate == false)
		return;


	switch (mState)
	{
	case enums::HUDState::None:
		break;
	case enums::HUDState::MoveBlink:
		MoveBlink(mChangeSize);
		break;
	case enums::HUDState::MoveTowards:
		MoveTowards();
		break;
	case enums::HUDState::Rotate:
		Rotate();
		break;
	case enums::HUDState::Size:
		Size();
		break;
	case enums::HUDState::TitleCapMove:
		TitleCapMove();
		break;
	case enums::HUDState::End:
		break;
	default:
		break;
	}



	//mElapsedTime += DT;

	//BaseRenderer* renderer = GetOwner()->GetComponent<BaseRenderer>();
	//Material* material = renderer->GetMaterial();

	//renderer::MaterialCB data = {};

	//data.fData1 = mFadeValue * (mElapsedTime / mTime);
	//data.iData1 = mFadeType;
	//data.iData2 = mFadeTextureType;


	//material->SetData(eGPUParam::Float_1, &data.fData1);
	//material->SetData(eGPUParam::Int_1, &data.iData1);
	//material->SetData(eGPUParam::Int_2, &data.iData2);
}

void HUD::OnFixedUpdate()
{
}

void HUD::OnRender()
{

}

void HUD::OnFontRender()
{

}

void HUD::OnClear()
{

}

void HUD::MoveBlink(Vector3 changeSize)
{

	if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::UP)))
	{
		Vector3 pos = this->GetComponent<Transform>()->GetPosition() + Vector3(0.0f, 1.f, 0.0f);
		this->GetComponent<Transform>()->SetPosition(pos);

		pos = mOriginScale * changeSize;  //Vector3(1.f, 0.9f, 1.0f)
		this->GetComponent<Transform>()->SetScale(pos);
		mState = HUDState::Size;
	}
	else if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::DOWN)))
	{
		Vector3 pos = this->GetComponent<Transform>()->GetPosition() + Vector3(0.0f, -1.f, 0.0f);
		this->GetComponent<Transform>()->SetPosition(pos);

		pos = mOriginScale * changeSize;
		this->GetComponent<Transform>()->SetScale(pos);
		mState = HUDState::Size;
	}
}

void HUD::MoveTowards()
{

}

void HUD::Rotate()
{
	if (mSpeed > 0)
	{
		mCurrentTime += DT;

		this->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, mCurrentTime * mSpeed));
	}
}

void HUD::Size()
{
	//mCurrentTime += DT;
	Vector3 scale = this->GetComponent<Transform>()->GetScale() + Vector3(DT, DT, 0.0f);

	this->GetComponent<Transform>()->SetScale(scale);

	if (scale.x > mOriginScale.x && scale.y > mOriginScale.y)
	{
		this->GetComponent<Transform>()->SetScale(mOriginScale);
		mCurrentTime = 0;
		mState = HUDState::MoveBlink;
	}
}

void HUD::TitleCapMove()
{

}

void HUD::PlayAnimation()
{

}