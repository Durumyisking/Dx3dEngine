#include "CompassNeedleScript.h"

CompassNeedleScript::CompassNeedleScript()
	:UIScript()
	,mPlayer(nullptr)
{

}

CompassNeedleScript::~CompassNeedleScript()
{

}

void CompassNeedleScript::Initialize()
{

}

void CompassNeedleScript::Update()
{
	if (mPlayer == nullptr)
		return;

	Vector3 rot = mPlayer->GetComponent<Transform>()->GetRotation();

	this->GetOwner()->GetComponent<Transform>()->SetRotationZ(-rot.y);
}