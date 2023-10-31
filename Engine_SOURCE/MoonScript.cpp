#include "MoonScript.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "InstancingContainer.h"

#include "TimeMgr.h"
#include "Panal.h"
#include "UIManager.h"
#include "PowerMoonScript.h"

MoonScript::MoonScript()
	:mState(ePowerMoonState::None)
	, mCurrentTime(0.f)
	, mOriginPos{}
	, mMaxRotZ(0.f)
	, mPowerMoonPanal{}
	, tr{}
{

}

MoonScript::~MoonScript()
{
}

void MoonScript::Initialize()
{
	mPowerMoonPanal = dynamic_cast<Panal*>((GETSINGLE(UIManager)->GetPanal(eUIType::Luna)));
	tr = this->GetOwner()->GetComponent<Transform>();
	mOwner = dynamic_cast<PowerMoonObject*>(this->GetOwner());
}

void MoonScript::Update()
{
	switch (mState)
	{
	case MoonScript::ePowerMoonState::None:
		break;
	case MoonScript::ePowerMoonState::Appear:
		Appear();
		break;
	case MoonScript::ePowerMoonState::Down:
		Down();
		break;
	case MoonScript::ePowerMoonState::Wait:
		Wait();
		break;
	default:
		break;
	}
}

void MoonScript::FixedUpdate()
{
}

void MoonScript::HitPayer()
{
	GetPowerMoon();
}

void MoonScript::GetPowerMoon()
{
	mOwner->IsHide(true);
	mOriginPos = {};
	mMaxRotZ = 0;
	mCurrentTime = 0;
	mState = ePowerMoonState::None;
	mPowerMoonPanal->GetScript<PowerMoonScript>()->GetPowerMoon();
	//this->GetOwner()->Die();
}

void MoonScript::PowerMoonAppears()
{
	mOwner->IsHide(false);
	mOriginPos = this->GetOwner()->GetPos();
	mState = ePowerMoonState::Appear;
}

void MoonScript::Appear()
{
	mCurrentTime += DT;

	tr->SetPhysicalRotation(Vector3(90.f, 0, mCurrentTime * 2000.f));
	tr->SetPhysicalPosition(Vector3(tr->GetPosition().x, tr->GetPosition().y + mCurrentTime * 0.1f, tr->GetPosition().z));

	if (tr->GetPosition().y >= mOriginPos.y + 3.f)
	{
		mState = ePowerMoonState::Down;
		mMaxRotZ = mCurrentTime * 2000.f;
		mCurrentTime = 0;
	}
}

void MoonScript::Down()
{
	mCurrentTime += DT;
	tr->SetPhysicalRotation(Vector3(90.f, 0, mMaxRotZ + mCurrentTime * 200.f));
	tr->SetPhysicalPosition(Vector3(tr->GetPosition().x, tr->GetPosition().y - mCurrentTime * 0.1f, tr->GetPosition().z));

	if (tr->GetPosition().y <= mOriginPos.y + 1.f)
	{
		tr->SetPhysicalPosition(mOriginPos + Vector3(0.f, 1.f, 0.f));
		mMaxRotZ += mCurrentTime * 200.f;
		mCurrentTime = 0;
		mState = ePowerMoonState::Wait;
	}
}

void MoonScript::Wait()
{
	mCurrentTime += DT;
	tr->SetPhysicalRotation(Vector3(90.f, 0, mMaxRotZ + mCurrentTime * 200.f));
}
