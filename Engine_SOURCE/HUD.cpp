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
