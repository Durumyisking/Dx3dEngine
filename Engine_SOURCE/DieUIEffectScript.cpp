#include "DieUIEffectScript.h"

DieUIEffectScript::DieUIEffectScript()
	:UIScript()
	, mPanal(nullptr)
{

}

DieUIEffectScript::~DieUIEffectScript()
{

}

void DieUIEffectScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void DieUIEffectScript::Update()
{

}

void DieUIEffectScript::UIReset()
{

}
