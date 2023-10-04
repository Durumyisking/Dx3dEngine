#include "PowerMoonScript.h"
#include "ImageUI.h"

PowerMoonScript::PowerMoonScript()
	:UIScript()
{

}

PowerMoonScript::~PowerMoonScript()
{

}

void PowerMoonScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void PowerMoonScript::Update()
{

}

void PowerMoonScript::GetPowerMoon()
{
	for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
	{
		if (mPanal->GetChilds()[i]->GetUIEnable())
		{
			continue;
		}
		else
		{
			mPanal->GetChilds()[i]->Activate();
			break;
		}
	}
}
