#pragma once
#include "UIScript.h"
#include "Panal.h"

class DieUIEffectScript : public UIScript
{
public:
	DieUIEffectScript();
	virtual ~DieUIEffectScript();

	virtual void Initialize() override;
	virtual void Update() override;

	void UIReset();

private:
	Panal* mPanal;
	
};