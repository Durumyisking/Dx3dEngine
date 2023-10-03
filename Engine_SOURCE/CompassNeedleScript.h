#pragma once
#include "UIScript.h"

class CompassNeedleScript : public UIScript
{
public:
	CompassNeedleScript();
	virtual ~CompassNeedleScript();

	virtual void Initialize() override;
	virtual void Update() override;

private:

};