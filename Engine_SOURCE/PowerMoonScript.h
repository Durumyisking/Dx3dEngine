#pragma once
#include "UIScript.h"
#include "Panal.h"

class PowerMoonScript : public UIScript
{
public:
	PowerMoonScript();
	virtual ~PowerMoonScript();

	virtual void Initialize() override;
	virtual void Update() override;

	void GetPowerMoon();


private:
	Panal* mPanal;

};