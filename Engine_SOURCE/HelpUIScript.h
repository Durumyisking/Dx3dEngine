#pragma once
#include "UIScript.h"
#include "Panal.h"

class HelpUIScript : public UIScript
{
	
	enum class State
	{
		None,
		Monster,
		NPC,
	};

public:
	HelpUIScript();
	virtual ~HelpUIScript();

	virtual void Initialize() override;
	virtual void Update() override;

private:
	Panal* mPanal;
	State mState;
};