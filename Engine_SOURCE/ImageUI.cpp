#include "ImageUI.h"

ImageUI::ImageUI()
	:UIBase(eUIType::None)
	, mActivate(false)
{
}

ImageUI::ImageUI(eUIType type)
	:UIBase(type)
	, mActivate(false)
{
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
	if (mActivate == false)
		return;
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
