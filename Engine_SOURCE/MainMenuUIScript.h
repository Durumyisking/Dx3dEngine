#pragma once
#include "Script.h"

class MainMenuUIScript : public Script
{
public:
	MainMenuUIScript();
	virtual ~MainMenuUIScript();


	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;

private:

};

