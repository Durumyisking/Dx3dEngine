#pragma once
#include "Script.h"

class LifeUIScript : public Script
{
public:
	LifeUIScript();
	virtual ~LifeUIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

private:

};

