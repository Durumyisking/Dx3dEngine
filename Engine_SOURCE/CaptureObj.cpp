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
	// �������� ������Ʈ�� �и� �� �� �̺�Ʈ
	// ����� ó�� Ư���ϰ� �и��� ������Ʈ��
	// �ش簴ü���ο��� �Լ� ������
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


