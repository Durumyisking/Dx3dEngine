#include "CompassUI.h"
#include "SpriteRenderer.h"

CompassUI::CompassUI()
	:UIBase(eUIType::None)
	, mActivate(false)
{

}

CompassUI::CompassUI(const std::wstring& key, eUIType type)
	:UIBase(type)
	, mActivate(false)
{
	SpriteRenderer* render = this->AddComponent<SpriteRenderer>(eComponentType::UI);
	render->SetMeshByKey(L"Rectmesh");
	render->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
}

CompassUI::~CompassUI()
{

}

void CompassUI::OnInit()
{

}

void CompassUI::OnActive()
{
	mActivate = true;
	this->Active();
}

void CompassUI::OnInActive()
{
	mActivate = false;
	this->Die();
}

void CompassUI::OnUpdate()
{

}

void CompassUI::OnFixedUpdate()
{

}

void CompassUI::OnRender()
{

}

void CompassUI::OnFontRender()
{

}

void CompassUI::OnClear()
{

}
