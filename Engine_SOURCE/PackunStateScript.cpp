#include "PackunStateScript.h"
#include "Monster.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"

PackunStateScript::PackunStateScript()
	:MonsterStateScript()
	, mFindRadius(6.f)
{
}

PackunStateScript::~PackunStateScript()
{
}

void PackunStateScript::Update()
{
	MonsterStateScript::Update();
}

void PackunStateScript::Idle()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	if (!mbAnimationRunning)
	{
		animator->Play(L"Wait");
		mbAnimationRunning = true;
	}

	if (mPlayer && !mMonster->IsCapture())
	{
		Transform* playerTr = mPlayer->GetTransform();
		Transform* tr = GetTransform();

		if (!playerTr || !tr)
			return;

		Vector3 playerPos = playerTr->GetPhysicalPosition();
		Vector3 pos = tr->GetPhysicalPosition();

		float distanc = 5.f;
		if (fabs(playerPos.y - pos.y) > distanc)
			return;

		Vector3 direction = playerPos - pos;
		if (direction.Length() > mFindRadius)
			return;

		// 월드 좌표상 회전이 0 일때 모델은 -90 도 방향을 바라보기때문에
		// 값의 90도를 빼주고 반전시킨다
		float angle = atan2(direction.z, direction.x);
		angle += math::toRadian(90.f);

		tr->SetPhysicalRotation(Vector3(0.f, math::toDegree(-angle), 0.f));

		mMonster->SetMonsterState(Monster::eMonsterState::Attack);
	}
}

void PackunStateScript::Move()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	PhysXRigidBody* rigidbody = GetOwner()->GetComponent<PhysXRigidBody>();
	if (rigidbody == nullptr)
		return;

	PhysicalMovement* moveMent = GetOwner()->GetComponent<PhysicalMovement>();
	if (moveMent == nullptr)
		return;

	// 캡처상태가아니면 조작할수 없다
	if (!mMonster->IsCapture())
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		return;
	}

	// 이동 키입력이 없을때
	if (GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::UP) && GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::DOWN)
		&& GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::LEFT) && GETSINGLE(InputMgr)->GetKeyNone(eKeyCode::RIGHT))
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		return;
	}

	Transform* tr = mMonster->GetComponent<Transform>();
	if (nullptr == tr)
		return;

	rigidbody->RemoveGravity();
	rigidbody->SetAirOff();

	/*if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::UP))
		rigidbody->AddForce(-tr->WorldForward() * 100.f);
	else if (GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::DOWN))
		rigidbody->AddForce(tr->WorldForward() * 100.f);*/


	if(GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::RIGHT))
		rigidbody->RightTrun();
	else if(GETSINGLE(InputMgr)->GetKeyDown(eKeyCode::LEFT))
		rigidbody->LeftTrun();
}

void PackunStateScript::Attack()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	std::wstring animationName = L"Attack";
	if (!mbAnimationRunning)
	{
		animator->Play(animationName);
		mbAnimationRunning = true;
	}
}

void PackunStateScript::SpecialSituation()
{

}

void PackunStateScript::Hit()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	std::wstring animationName = L"AttackHit";
	if (!mbAnimationRunning)
	{
		animator->Play(animationName);
		mbAnimationRunning = true;
	}

	// 캡처상태일때 맞았을경우 플레이어의 Hp가 감소
	if (mPlayer)
	{
		//mPlayer->DownHp();
	}
	// 캡처 X
	else
	{
		//mMonster->DownHp();
	}
}

void PackunStateScript::Groggy()
{
	// 해당 Groggy 함수에서는 공격후 딜레이 애니메이션을
	// 재생하고 있습니다.
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	std::wstring animationName = L"HackWait";
	if (!mbAnimationRunning)
	{
		animator->Play(animationName);
		mbAnimationRunning = true;
	}

	if (mPlayer)
	{
		Transform* playerTr = mPlayer->GetTransform();
		Transform* tr = GetTransform();

		if (!playerTr || !tr)
			return;

		Vector3 playerPos = playerTr->GetPhysicalPosition();
		Vector3 pos = tr->GetPhysicalPosition();

		float distanc = 5.f;
		if (fabs(playerPos.y - pos.y) > distanc)
			return;

		Vector3 direction = playerPos - pos;
		if (direction.Length() > mFindRadius)
			return;

		// 월드 좌표상 회전이 0 일때 모델은 90 도 방향을 바라보기때문에
		// 값의 90도를 빼주고 반전시킨다
		float angle = atan2(direction.z, direction.x);
		angle += math::toRadian(90.f);

		tr->SetPhysicalRotation(Vector3(0.f, math::toDegree(-angle), 0.f));
	}
}

void PackunStateScript::Die()
{
	BoneAnimator* animator = mMonster->GetComponent<BoneAnimator>();
	if (animator == nullptr)
		return;

	std::wstring animationName = L"PressDown";
	if (!mbAnimationRunning)
	{
		animator->Play(animationName, false);
		mbAnimationRunning = true;
	}
}
