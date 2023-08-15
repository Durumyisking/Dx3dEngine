#include "UIBase.h"

UIBase::UIBase(eUIType type)
	:mUIType(type)
	,mUIbFullScreen(false)
	,mbUIEnable(false)
	,mUIParent(nullptr)
{

}

UIBase::~UIBase()
{

}

void UIBase::Initialize()
{
	GameObj::Initialize();
	OnInit();
}

void UIBase::Active()
{
	mbUIEnable = true;
	OnActive();
}

void UIBase::InActive()
{
	mbUIEnable = false;
	OnInActive();
}

void UIBase::Update()
{
	if (mbUIEnable == false)
		return;

	GameObj::Update();
	OnUpdate();
}

void UIBase::FixedUpdate()
{
	if (mbUIEnable == false)
		return;

	GameObj::FixedUpdate();
	OnFixedUpdate();
}

void UIBase::Render()
{
	if (mbUIEnable == false)
		return;

	GameObj::Render();
	OnRender();
}

void UIBase::FontRender()
{
	if (mbUIEnable == false)
		return;

	OnFontRender();
}

void UIBase::UIClear()
{
	OnClear();
}
