#include "CapMoveScript.h"
#include "TimeMgr.h"
#include "ImageUI.h"

CapMoveScript::CapMoveScript()
	:UIScript()
	, mTargetPos{}
	, mOriginPos{}
	, mCurrentTime(0.0f)
	, mCount(0)
	, mbGoAndReturn(false)
{

}

CapMoveScript::~CapMoveScript()
{

}

void CapMoveScript::Initialize()
{
	mTargetPos = this->GetOwner()->GetComponent<Transform>()->GetPosition() + Vector3(0.05f, 0.0f, 0.0f);
}

void CapMoveScript::Update()
{
	if (mTargetPos == Vector3::Zero)
		return;

	if (mCount >= 2)
	{
		mCurrentTime += DT;

		if (mCurrentTime >= 1)
		{
			mCount = 0;
			mCurrentTime = 0;
		}

		return;
	}

	Transform* tr = this->GetOwner()->GetComponent<Transform>();

	Vector3 pos = tr->GetPosition();
	Vector3 scale = tr->GetScale();


	if (mbGoAndReturn == false)
	{
		if (pos.x > -0.25f)
		{
			mbGoAndReturn = true;
		}

		pos.x += DT * 0.5f;
		scale.x -= DT * 0.5f;
	}
	else if (mbGoAndReturn)
	{
		if (pos.x < -0.3f)
		{
			mbGoAndReturn = false;
			mCount++;
		}

		pos.x -= DT * 0.5f;
		scale.x += DT * 0.5f;
	}

	tr->SetPosition(pos);
	tr->SetScale(scale);
}
