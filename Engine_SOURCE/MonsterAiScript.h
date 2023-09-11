#pragma once
#include "AIScript.h"

class MonsterAiScript : public AiScript
{
public:
	MonsterAiScript();
	virtual ~MonsterAiScript();


	virtual void Update() override;
	virtual void FixedUpdate() override;

	bool IsRunning();
private:

};

