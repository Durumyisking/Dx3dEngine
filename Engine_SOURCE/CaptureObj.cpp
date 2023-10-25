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
	if (mCap && KEY_DOWN(P))  // 굼바 탑 처리를 위한 코드입니다. 굼바 탑의 조작권은 1층 굼바가 가지고 있고, 모자,수염은 맨 윗굼바에 있기 때문에 unCapture하려면 당장은 이렇게 구현해야 했음
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

	Vector3 CapturedObjectPosition = monsterTr->GetPhysicalPosition();
	CapturedObjectPosition.y += (monster->GetPhysical()->GetGeometrySize().y * 4.f);

	playertr->SetPhysicalPosition(CapturedObjectPosition); // 캡처에서 빠져나오면서 충돌을 피하기 위해 캡처대상의 머리 위에서 나가게 처리한다.
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
		// todo : 위험한 코드입니다 captureobj랑 gameobj는 별개의 오브젝트임 적절한 방법 찾아서 고쳐야함
		mCap->SetOwner(dynamic_cast<GameObj*>(this));
	}
}

void CaptureObj::ClearCaptureData(bool copyCap)
{
	mPlayer = nullptr;
	if(!copyCap)
		mCap = nullptr;
}




