#pragma once
#include "UIScript.h"

class MainMenuScript : public UIScript
{
public:
	MainMenuScript();
	virtual ~MainMenuScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

private:

};

