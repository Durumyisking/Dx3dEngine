#include "Button.h"
#include "InputMgr.h"

Button::Button()
	:UIBase(eUIType::Button)
{
	mOnClick = std::bind(&Button::Click, this);
}

Button::Button(eUIType type)
	:UIBase(type)
{

}

Button::~Button()
{

}

void Button::OnInit()
{

}

void Button::OnActive()
{

}

void Button::OnInActive()
{

}

void Button::OnUpdate()
{

}

void Button::OnFixedUpdate()
{

}

void Button::OnRender()
{

}

void Button::OnFontRender()
{

}

void Button::OnClear()
{

}

void Button::Click()
{

}
