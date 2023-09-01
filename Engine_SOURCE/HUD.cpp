#include "HUD.h"

HUD::HUD()
	:UIBase(eUIType::HP)
{

}

HUD::HUD(eUIType type)
	:UIBase(type)
{

}

HUD::~HUD()
{
}

void HUD::OnInit()
{
}

void HUD::OnActive()
{
}

void HUD::OnInActive()
{
	this->Die();
}

void HUD::OnTick()
{
}

void HUD::OnRender()
{

}

void HUD::OnClear()
{
}
