#include "Monster.h"
#include "MonsterAiScript.h"
#include "MonsterStateScript.h"
#include "BoneAnimator.h"

Monster::Monster()
	: DynamicObject()
	, mMonsterState(eMonsterState::Idle)
{
}

Monster::~Monster()
{
}

void Monster::Initialize()
{
	//StateInfoSetting
	mStateInfo.resize(static_cast<int>(eMonsterState::Die) + 1);
	

	// Animator
	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	if (animator != nullptr)
	{
		boneAnimatorInit(animator);
	}
	GameObj::Initialize();
}

void Monster::Update()
{
	GameObj::Update();
	CaptureObj::Update();
}

void Monster::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void Monster::SetMonsterState(eMonsterState monsterState)
{
	std::set<UINT>::iterator iter;
	iter = mStateInfo[static_cast<UINT>(mMonsterState)].mLockState.find(static_cast<UINT>(monsterState));
	if (iter != mStateInfo[static_cast<UINT>(mMonsterState)].mLockState.end())
		return;

	mMonsterState = monsterState;

	MonsterStateScript* script = GetScript<MonsterStateScript>();
	if (script)
		script->Reset();
}
