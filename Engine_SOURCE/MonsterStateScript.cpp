#include "MonsterStateScript.h"
#include "Monster.h"

#include "SceneMgr.h"
#include "Scene.h"

MonsterStateScript::MonsterStateScript()
	: Script()
	, mStateEventList{}
	, mMonster(nullptr)
	, mPlayer(nullptr)
	, mbAnimationRunning(false)
	, mbTurnLeft(false)
{
	// �޸� ���� Ȯ��
	mStateEventList.reserve(static_cast<UINT>(Monster::eMonsterState::Die) + 1);

	// �̺�Ʈ ���ε� (this�� �ִ� ������ �� �Լ����� ���ڿ� this�� ���� ������)
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Idle, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Move, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Jump, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Fall, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Land, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Turn, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Chase, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Attack, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::SpecialSituation, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Hit, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Groggy, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Die, this));
}

MonsterStateScript::~MonsterStateScript()
{
	mStateEventList.clear();
}

void MonsterStateScript::Update()
{
	if (mMonster == nullptr)
		return;

	if (!mPlayer)
		mPlayer = GETSINGLE(SceneMgr)->GetActiveScene()->GetPlayer();

	UINT iState = static_cast<UINT>(mMonster->GetMonsterState());
	// enum ���¿� ��Ī�Ǵ� �迭�� �ε����� ����
	mStateEventList[iState]();
	
	Script::Update();
}

void MonsterStateScript::Initialize()
{
	// Owner �� ��ȯ
	if (GetOwner())
		mMonster = dynamic_cast<Monster*>(GetOwner());

}

void MonsterStateScript::Idle()
{	
	if (mPlayer)
	{
		float dist = Vector3::Distance(GETSINGLE(SceneMgr)->GetActiveScene()->GetPlayer()->GetWorldPos(), GetOwnerWorldPos());
		if (mMonster->GetGetRecognizeRadius() > dist)
		{
			mMonster->SetIsFoundPlayer(true);
			mMonster->SetMonsterState(Monster::eMonsterState::Turn);

			Vector3 dirToPlayer_XYZ = mPlayer->GetWorldPos() - GetTransform()->WorldForward();
			Vector2 dirToPlayer = { dirToPlayer_XYZ.x, dirToPlayer_XYZ.z};
			Vector2 worldForward = { GetTransform()->WorldForward().x, GetTransform()->WorldForward().z };
			dirToPlayer.Normalize();
			float rotCosTheta = dirToPlayer.Dot(worldForward);
			if (rotCosTheta > 0.f)
			{
				mbTurnLeft = true;
			}
		}
		else
		{
			mMonster->SetIsFoundPlayer(false);
			mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		}
	}
}
