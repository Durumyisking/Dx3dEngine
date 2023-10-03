#include "ImageUI.h"
#include "TimeMgr.h"
#include "SpriteRenderer.h"

ImageUI::ImageUI()
	:UIBase(eUIType::None)
	, mCurrentTime(0.0f)
	, mActivate(false)
{
}

ImageUI::ImageUI(eUIType type)
	:UIBase(type)
	, mCurrentTime(0.0f)
	, mActivate(false)
{
}

ImageUI::ImageUI(const std::wstring& key, eUIType type)
	:UIBase(type)
	, mCurrentTime(0.0f)
	, mActivate(false)
{
	SpriteRenderer* hudRender = this->AddComponent<SpriteRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
}

ImageUI::~ImageUI()
{
}

void ImageUI::OnInit()
{
}

void ImageUI::OnActive()
{
	mActivate = true;
	this->Active();
}

void ImageUI::OnInActive()
{
	mActivate = false;
	this->Die();
}

void ImageUI::OnUpdate()
{

}

void ImageUI::OnFixedUpdate()
{

}

void ImageUI::OnRender()
{

}

void ImageUI::OnFontRender()
{

}

void ImageUI::OnClear()
{

}

void ImageUI::SetColor(Vector4 color, bool isColor)
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