#pragma once
#include "UIScript.h"

class RotationUIScript : public UIScript
{
public:
	RotationUIScript();
	virtual ~RotationUIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

private:

};

