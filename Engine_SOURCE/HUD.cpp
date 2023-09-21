#include "HUD.h"
#include "TimeMgr.h"
#include "InputMgr.h"

#include "SpriteRenderer.h"

HUD::HUD()
	:UIBase(eUIType::HP)
	, mSpeed(0)
	, mCurrentTime(0)
	, mChangeSize(Vector3::One)
	, mTargetPos(Vector3::Zero)
	, mState(eHUDState::None)
	, mActivate(false)
	, mbStop(false)
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
	, mState(eHUDState::None)
	, mActivate(false)
	, mbStop(false)
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

	if (mbStop)
		return;

	switch (mState)
	{
	case enums::eHUDState::None:
		break;
	case enums::eHUDState::MoveBlink:
		MoveBlink(mChangeSize);
		break;
	case enums::eHUDState::MoveTowards:
		MoveTowards();
		break;
	case enums::eHUDState::Rotate:
		Rotate();
		break;
	case enums::eHUDState::Size:
		Size();
		break;
	case enums::eHUDState::TitleCapMove:
		TitleCapMove();
		break;
	case enums::eHUDState::End:
		break;
	case enums::eHUDState::Hit:

		break;
	default:
		break;
	}
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
		mState = eHUDState::Size;
	}
	else if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::DOWN)))
	{
		Vector3 pos = this->GetComponent<Transform>()->GetPosition() + Vector3(0.0f, -1.f, 0.0f);
		this->GetComponent<Transform>()->SetPosition(pos);

		pos = mOriginScale * changeSize;
		this->GetComponent<Transform>()->SetScale(pos);
		mState = eHUDState::Size;
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
		mState = eHUDState::MoveBlink;
	}
}

void HUD::TitleCapMove()
{

}

void HUD::PlayAnimation()
{

}


void HUD::SetColor(Vector4 color, bool isColor)
{
	SpriteRenderer* renderer = this->GetComponent<SpriteRenderer>();
	Material* material = renderer->GetMaterial();

	renderer::MaterialCB data = {};
	data.bool3 = isColor;
	data.xyzw1 = color;
	material->SetData(eGPUParam::Bool_3, &data.bool3);
	material->SetData(eGPUParam::Vector4_1, &data.xyzw1);

	mbColor = true;
	mCurrentTime = 0;
}
