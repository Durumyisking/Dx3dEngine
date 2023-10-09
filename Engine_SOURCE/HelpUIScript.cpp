#include "HelpUIScript.h"

HelpUIScript::HelpUIScript()
	:UIScript()
	,mPanal(nullptr)
	,mState(State::None)
{

}

HelpUIScript::~HelpUIScript()
{

}

void HelpUIScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void HelpUIScript::Update()
{

}