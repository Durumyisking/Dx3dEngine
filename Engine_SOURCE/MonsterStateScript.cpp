#include "MonsterStateScript.h"
#include "Monster.h"

MonsterStateScript::MonsterStateScript()
	: Script()
	, mStateEventList{}
	, mMonster(nullptr)
{
	// 메모리 공간 확보
	mStateEventList.reserve(static_cast<UINT>(Monster::eMonsterState::Die));

	// 이벤트 바인딩
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
	// enum 상태와 매칭되는 배열을 인덱스로 접근
	mStateEventList[iState]();

	Script::Update();
}

void MonsterStateScript::Initialize()
{
	// Owner 형 변환
	if (GetOwner())
		mMonster = dynamic_cast<Monster*>(GetOwner());
}
