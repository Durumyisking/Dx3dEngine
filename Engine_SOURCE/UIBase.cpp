#include "UIBase.h"


Vector3 mUIScreenPos;
Vector3 mUIPos;
Vector3 mUISize;

UIBase::UIBase(eUIType type)
	:mUIType(type)
	,mUIbFullScreen(false)
	,mbUIEnable(true)
	,mUIParent(nullptr)
	,mUIScreenPos(Vector3::Zero)
	,mUIPos(Vector3::Zero)
	,mUISize(Vector3::One)
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

	for (UIBase* child : mChilds)
	{
		child->mbUIEnable = true;
		child->OnActive();
	}
}

void UIBase::InActive()
{
	for (UIBase* child : mChilds)
	{
		child->mbUIEnable = false;
		child->OnInActive();
	}
	OnInActive();
	mbUIEnable = false;
}

void UIBase::Update()
{
	if (mbUIEnable == false)
		return;

	GameObj::Update();
	OnUpdate();

	if (mUIParent)
		mUIPos = (mUIParent->GetPos());
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

void UIBase::Addchild(UIBase* uiBase)
{
	mChilds.push_back(uiBase);
	uiBase->mUIParent = this;
}