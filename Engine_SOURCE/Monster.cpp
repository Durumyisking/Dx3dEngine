#include "Monster.h"
<<<<<<< HEAD

Monster::Monster()
=======
#include "MonsterAiScript.h"
#include "MonsterStateScript.h"
#include "BoneAnimator.h"

Monster::Monster()
	: DynamicObject()
	, mMonsterState(eMonsterState::Idle)
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
{
}

Monster::~Monster()
{
}

void Monster::Initialize()
{
<<<<<<< HEAD
=======
	//StateInfoSetting
	mStateInfo.resize(static_cast<int>(eMonsterState::Die) + 1);
	

	// Animator
	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	if (animator != nullptr)
	{
		boneAnimatorInit(animator);
	}
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
}

void Monster::Update()
{
<<<<<<< HEAD
=======
	GameObj::Update();
	CaptureObj::Update();
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
}

void Monster::FixedUpdate()
{
<<<<<<< HEAD
}

void Monster::Render()
{
}

void Monster::FontRender()
{
}

void Monster::OnCollisionEnter(GameObj* gameObject)
{
}

void Monster::OnTriggerEnter(GameObj* gameObject)
{
}

void Monster::OnTriggerExit(GameObj* gameObject)
{
=======
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
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
}
