#pragma once
#include "Script.h"

class AiScript : public Script
{
public:
	AiScript();
	virtual ~AiScript();

	virtual void Update() override;
private:
};

