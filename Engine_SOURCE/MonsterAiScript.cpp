#include "MonsterAiScript.h"
#include "Monster.h"


MonsterAiScript::MonsterAiScript()
{
}

MonsterAiScript::~MonsterAiScript()
{
}

void MonsterAiScript::Update()
{
	if (IsRunning() == false)
		return;

	AiScript::Update();
}

void MonsterAiScript::FixedUpdate()
{
	if (IsRunning() == false)
		return;

	AiScript::FixedUpdate();
}

bool MonsterAiScript::IsRunning()
{
	if (GetOwner() == nullptr)
		return false;

	Monster* monster = dynamic_cast<Monster*>(GetOwner());
	if (monster == nullptr)
		return false;

	if (monster->GetCapture())
		return false;

	return true;
}
