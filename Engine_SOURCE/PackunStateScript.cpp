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

		// ���� ��ǥ�� ȸ���� 0 �϶� ���� -90 �� ������ �ٶ󺸱⶧����
		// ���� 90���� ���ְ� ������Ų��
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
	if (!rigidbody)
		return;

	PhysicalMovement* moveMent = GetOwner()->GetComponent<PhysicalMovement>();
	if (moveMent == nullptr)
		return;

	if (!mMonster->IsCapture())
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		return;
	}

	if (GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::UP) || GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::DOWN)
		|| GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::LEFT) || GETSINGLE(InputMgr)->GetKeyUp(eKeyCode::RIGHT))
	{
		mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		return;
	}

	Transform* tr = mMonster->GetComponent<Transform>();
	if (nullptr == tr)
		return;

	bool able = false;
	auto Input_DownFunC = [&](eKeyCode key, eKeyCode mult_key, math::Vector3 rotation)
		{
			if (able)
				return;

			if (GETSINGLE(InputMgr)->GetKeyDown(key))
			{
				if (GETSINGLE(InputMgr)->GetKeyDown(mult_key))
				{
					tr->SetPhysicalRotation(rotation);
					able = true;
				}
			}
		};

	Input_DownFunC(eKeyCode::UP, eKeyCode::RIGHT, math::Vector3(0.0f, -135.f, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::LEFT, math::Vector3(0.0f, -225, 0.0f));
	Input_DownFunC(eKeyCode::UP, eKeyCode::UP, math::Vector3(0.0f, -180.f, 0.0f));

	Input_DownFunC(eKeyCode::DOWN, eKeyCode::RIGHT, math::Vector3(0.0f, -45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::LEFT, math::Vector3(0.0f, 45.f, 0.0f));
	Input_DownFunC(eKeyCode::DOWN, eKeyCode::DOWN, math::Vector3(0.0f, 0.f, 0.0f));

	Input_DownFunC(eKeyCode::LEFT, eKeyCode::LEFT, math::Vector3(0.0f, 90.f, 0.0f));
	Input_DownFunC(eKeyCode::RIGHT, eKeyCode::RIGHT, math::Vector3(0.0f, -90.f, 0.0f));
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
}

void PackunStateScript::Groggy()
{
	// �ش� Groggy �Լ������� ������ ������ �ִϸ��̼���
	// ����ϰ� �ֽ��ϴ�.
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

		// ���� ��ǥ�� ȸ���� 0 �϶� ���� 90 �� ������ �ٶ󺸱⶧����
		// ���� 90���� ���ְ� ������Ų��
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
