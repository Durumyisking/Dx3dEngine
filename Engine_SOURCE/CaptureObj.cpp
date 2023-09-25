#include "CaptureObj.h"
#include "InputMgr.h"

#include "Player.h"
#include "PhysXRigidBody.h"

#include "MarioCap.h"
#include "Physical.h"

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

	mPlayer->SetMarioCap(mObject);
	mPlayer->Active();
	mObject->Pause();

	// �������� ���ڸ� ������
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
	model->MeshRenderSwtich(L"Cap__CapMT-mesh", true);

	GameObj* monster = dynamic_cast<GameObj*>(this);
	Transform* monsterTr = nullptr;
	if(monster)
		monsterTr = monster->GetTransform();

	if (monsterTr == nullptr)
		return;

	Transform* tr = mObject->GetTransform();
	Transform* playertr = mPlayer->GetTransform();

	playertr->SetPhysicalPosition(monsterTr->GetPhysicalPosition());

	PhysXRigidBody* rigidbody = mPlayer->GetPhysXRigidBody();
	rigidbody->SetVelocity(Vector3::Zero);
	rigidbody->ApplyGravity();
	rigidbody->SetAirOn();

	rigidbody->SetMaxVelocity_Y(10.f);
	rigidbody->AddForce(Vector3(0.f, 9999999.f, 0.f));
}


