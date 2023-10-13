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

	playertr->SetPhysicalPosition(monsterTr->GetPhysicalPosition());
	playertr->SetPhysicalRotation(Vector3(0.f, 0.0f, 0.f));

	mPlayer->SetPlayerState(Player::ePlayerState::Idle);
	mPlayer->GetScript<PlayerStateScript>()->SetHavingCap(true);

	BoneAnimator* animator = mPlayer->GetBoneAnimator();
	PhysXRigidBody* rigidbody = mPlayer->GetPhysXRigidBody();

	animator->Play(L"Jump", false);
	
	rigidbody->SetMaxVelocity_Y(10.f);
	rigidbody->ApplyGravity();
	rigidbody->SetAirOn();
	rigidbody->AddForce(math::Vector3(10000.f, 10000.f, 0.0f));


	mPlayer->SetPlayerState(Player::ePlayerState::Jump);
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




