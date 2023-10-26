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
	if (mCap && KEY_DOWN(P))  // ���� ž ó���� ���� �ڵ��Դϴ�. ���� ž�� ���۱��� 1�� ���ٰ� ������ �ְ�, ����,������ �� �����ٿ� �ֱ� ������ unCapture�Ϸ��� ������ �̷��� �����ؾ� ����
		Divide();

	if (mbCapture)
	{
		CaptureEvent();

		if(KEY_DOWN(P))
			OffCapture();
	}
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

	mCap->Pause();

	mCap = nullptr;
	mPlayer = nullptr;
}

void CaptureObj::CopyCaptureData(CaptureObj* other, bool copyCap)
{
	mPlayer = other->GetPlayer();
	if (copyCap)
	{
		mCap = other->GetCap();
		other->ClearCaptureData(copyCap);
		// todo : ������ �ڵ��Դϴ� captureobj�� gameobj�� ������ ������Ʈ�� ������ ��� ã�Ƽ� ���ľ���
		mCap->SetOwner(dynamic_cast<GameObj*>(this));
	}
}

void CaptureObj::ClearCaptureData(bool copyCap)
{
	mPlayer = nullptr;
	if(!copyCap)
		mCap = nullptr;
}




