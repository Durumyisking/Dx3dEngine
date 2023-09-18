#include "MonsterStateScript.h"
#include "Monster.h"

MonsterStateScript::MonsterStateScript()
	: Script()
	, mStateEventList{}
	, mMonster(nullptr)
	, mbAnimationRunning(false)
{
	// 메모리 공간 확보
	mStateEventList.reserve(static_cast<UINT>(Monster::eMonsterState::Die) + 1);

	// 이벤트 바인딩 (this를 넣는 이유는 각 함수들의 인자에 this가 들어가기 때문임)
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Idle, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Move, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Jump, this));
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
