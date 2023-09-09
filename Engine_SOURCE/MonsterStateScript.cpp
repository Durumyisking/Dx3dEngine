#include "MonsterStateScript.h"
#include "Monster.h"

MonsterStateScript::MonsterStateScript()
	: Script()
	, mStateEventList{}
	, mMonster(nullptr)
{
	// �޸� ���� Ȯ��
	mStateEventList.reserve(static_cast<UINT>(Monster::eMonsterState::Die));

	// �̺�Ʈ ���ε�
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Idle, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Move, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Jump, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Attack, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::SpecialCast, this));
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
