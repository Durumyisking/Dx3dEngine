#include "CaptureObj.h"
#include "InputMgr.h"

#include "Player.h"
#include "PhysXRigidBody.h"

CaptureObj::~CaptureObj()
{
}

void CaptureObj::Update()
{
	if (!mbCapture)
		return;

	if (KEY_DOWN(P))
		Divide();

	CaptureEvent();
}

void CaptureObj::Divide()
{
	// 마리오와 오브젝트가 분리 될 때 이벤트
	// 라바콘 처럼 특수하게 분리될 오브젝트는
	// 해당객체내부에서 함수 재정의
	OffCapture();

	if (mObject == nullptr)
		return;

	if (mPlayer == nullptr)
		return;

	Transform* tr = mObject->GetTransform();
	Transform* playertr = mPlayer->GetTransform();

	playertr->SetPhysicalPosition(tr->GetPhysicalPosition());

	PhysXRigidBody* rigidbody = mPlayer->GetPhysXRigidBody();
	rigidbody->SetVelocity(Vector3::Zero);
	rigidbody->ApplyGravity();
	rigidbody->SetAirOn();

	rigidbody->SetMaxVelocity_Y(20.f);
	rigidbody->AddForce(Vector3(0.f, 9999999.f, 0.f));
}


