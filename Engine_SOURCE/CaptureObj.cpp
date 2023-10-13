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
	// 마리오와 오브젝트가 분리 될 때 이벤트
	// 라바콘 처럼 특수하게 분리될 오브젝트는
	// 해당객체내부에서 함수 재정의
	OffCapture();
	DivideEvent();

	if (mCap == nullptr)
		return;

	if (mPlayer == nullptr)
		return;

	mPlayer->SetMarioCap(mCap);
	mPlayer->Active();
	mPlayer->UnCapturingProcess();
	

	// 마리오의 모자를 씌워줌
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

	// todo : 위험한 코드입니다 captureobj랑 gameobj는 별개의 오브젝트임 적절한 방법 찾아서 고쳐야함
	mCap->SetOwner(dynamic_cast<GameObj*>(this));
}

void CaptureObj::ClearCaptureData()
{
	mPlayer = nullptr;
	mCap = nullptr;
}




