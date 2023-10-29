#include "BlockBrickScript.h"
#include "GameObj.h"
#include "BoneAnimator.h"

BlockBrickScript::BlockBrickScript()
	:mState(eState::Idle)
{

}

BlockBrickScript::~BlockBrickScript()
{
}

void BlockBrickScript::Initialize()
{

}

void BlockBrickScript::Update()
{
	switch (mState)
	{
	case BlockBrickScript::eState::Idle:
		break;
	case BlockBrickScript::eState::Distroy:
		this->GetOwner()->GetComponent<BoneAnimator>()->Play(L"ReactionHipDrop", false);
		mState = eState::Die;
		break;
	case BlockBrickScript::eState::Die:
		break;
	default:
		break;
	}
}

void BlockBrickScript::FixedUpdate()
{

}

void BlockBrickScript::GetHit()
{
	if (mState != eState::Idle)
		return;

	mState = eState::Distroy;
}