#pragma once
#include "UIScript.h"

class CapMoveScript : public UIScript
{
public:
	CapMoveScript();
	virtual ~CapMoveScript();

	virtual void Initialize() override;
	virtual void Update() override;

private:
	Vector3 mTargetPos;
	Vector3 mOriginPos;

	float mCurrentTime;

	int mCount;

	bool mbGoAndReturn;

};