#include "MilestoneScript.h"



MilestoneScript::MilestoneScript()
	:UIScript()
	,mPanal(nullptr)
{

}

MilestoneScript::~MilestoneScript()
{

}

void MilestoneScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void MilestoneScript::Update()
{

}