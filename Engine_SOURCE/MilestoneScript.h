#pragma once
#include "UIScript.h"
#include "Panal.h"

class MilestoneScript : public UIScript
{
public:
	MilestoneScript();
	virtual ~MilestoneScript();

	virtual void Initialize() override;
	virtual void Update() override;

private:
	Panal* mPanal;
};