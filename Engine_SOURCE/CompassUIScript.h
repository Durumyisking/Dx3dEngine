#pragma once
#include "UIScript.h"

class CompassUIScript : public UIScript
{
public:
	CompassUIScript();
	virtual ~CompassUIScript();

	virtual void Initialize() override;
	virtual void Update() override;

private:
	Transform* mCameraTr;
	Transform* mTr;
};