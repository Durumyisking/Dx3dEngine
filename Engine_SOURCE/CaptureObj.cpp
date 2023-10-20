#include "CaptureObj.h"
#include "InputMgr.h"

#include "Player.h"
#include "PlayerStateScript.h"
#include "PhysXRigidBody.h"

#include "MarioCap.h"
#include "Physical.h"

#include "BoneAnimator.h"
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
	DivideEvent();

	if (mCap == nullptr)
		return;

	if (mPlayer == nullptr)
		return;

	mPlayer->SetMarioCap(mCap);
	mPlayer->Active();
	mPlayer->UnCapturingProcess();

	// �������� ���ڸ� ������
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
	model->MeshRenderSwtich(L"Cap__CapMT-mesh", true);

	GameObj* monster = dynamic_cast<GameObj*>(this);
	Transform* monsterTr = nullptr;
	if(monster)
		monsterTr = monster->GetTransform();

	if (monsterTr == nullptr)
		return;

	Transform* tr = mCap->GetTransform();
	Transform* playertr = mPlayer->GetTransform();

	Vector3 CapturedObjectPosition = monsterTr->GetPhysicalPosition();
	CapturedObjectPosition.y += (monster->GetPhysical()->GetGeometrySize().y * 4.f);

	playertr->SetPhysicalPosition(CapturedObjectPosition); // ĸó���� ���������鼭 �浹�� ���ϱ� ���� ĸó����� �Ӹ� ������ ������ ó���Ѵ�.
	playertr->SetPhysicalRotation(monsterTr->GetPhysicalRotation());

	mPlayer->GetScript<PlayerStateScript>()->SetHavingCap(true);

	BoneAnimator* animator = mPlayer->GetBoneAnimator();
	PhysXRigidBody* rigidbody = mPlayer->GetPhysXRigidBody();

	animator->Play(L"Jump", false);
	
	rigidbody->SetMaxVelocity_Y(17.f);
	rigidbody->ApplyGravity();
	rigidbody->SetAirOn();
	Vector3 forwardUp = -monsterTr->Forward();
	forwardUp.y = 1.f;
	rigidbody->AddForce(forwardUp * 100000.f);

	mPlayer->SetPlayerState(Player::ePlayerState::Jump);

	renderer::mainCamera->SetTarget(mPlayer);

	mCap = nullptr;

}

void CaptureObj::CopyCaptureData(CaptureObj* other)
{
	mPlayer = other->GetPlayer();
	mCap = other->GetCap();
	other->ClearCaptureData();

	// todo : ������ �ڵ��Դϴ� captureobj�� gameobj�� ������ ������Ʈ�� ������ ��� ã�Ƽ� ���ľ���
	mCap->SetOwner(dynamic_cast<GameObj*>(this));
}

void CaptureObj::ClearCaptureData()
{
	mPlayer = nullptr;
	mCap = nullptr;
}




