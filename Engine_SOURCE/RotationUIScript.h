#pragma once
#include "Script.h"

class RotationUIScript : public Script
{
public:
	RotationUIScript();
	virtual ~RotationUIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

private:

};

