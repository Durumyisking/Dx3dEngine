#include "Monster.h"
#include "MonsterAiScript.h"
#include "MonsterStateScript.h"
#include "BoneAnimator.h"

Monster::Monster()
	: GameObj()
	, mMonsterState(eMonsterState::Idle)
{
}

Monster::~Monster()
{
}

void Monster::Initialize()
{
	// Animator
	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	if (animator != nullptr)
	{
		boneAnimatorInit(animator);
	}
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
	mMonsterState = monsterState;

	MonsterStateScript* script = GetScript<MonsterStateScript>();
	if (script)
		script->Reset();
}
