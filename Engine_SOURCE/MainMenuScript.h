#pragma once
#include "Script.h"

class MainMenuScript : public Script
{
public:
	MainMenuScript();
	virtual ~MainMenuScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

private:

};

