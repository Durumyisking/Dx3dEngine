#pragma once
#include "UIScript.h"

class CapEyeScript : public UIScript
{
public:
	CapEyeScript();
	virtual ~CapEyeScript();

	virtual void Initialize() override;
	virtual void Update() override;


	void CapAnimation();
private:
	Transform* mTr;

	float mCurrentTime;

	int mAnimationCnt;

	bool mbEyeblink;
	bool mbIdle;

};