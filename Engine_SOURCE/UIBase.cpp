#include "UIBase.h"
#include "Material.h"
#include "SpriteRenderer.h"


Vector3 mUIScreenPos;
Vector3 mUIPos;
Vector3 mUISize;

UIBase::UIBase(eUIType type)
	:mUIType(type)
	, mUIbFullScreen(false)
	, mbUIEnable(true)
	, mbUIOn(true)
	, mbColor(false)
	, mUIParent(nullptr)
	, mUIScreenPos(Vector3::Zero)
	, mUIPos(Vector3::Zero)
	, mUISize(Vector3::One)
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

void UIBase::Activate()
{
	mbUIEnable = true;
	OnActive();

	for (UIBase* child : mChilds)
	{	
		if (child->GetUIIsOn())
		{
			child->OnActive();
			child->mbUIEnable = true;
		}
		else
		{
			continue;
		}
	}
}

void UIBase::InActivate()
{
	for (UIBase* child : mChilds)
	{
		child->OnInActive();
		child->mbUIEnable = false;
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

void UIBase::ChangeTexture(const std::wstring& key)
{
	if (this->GetComponent<SpriteRenderer>() == nullptr)
		return;

	SpriteRenderer* render = this->GetComponent<SpriteRenderer>();

	Texture* tex = (GETSINGLE(ResourceMgr)->Find<Texture>(key));

	render->GetMaterial()->SetTexture(tex);
}