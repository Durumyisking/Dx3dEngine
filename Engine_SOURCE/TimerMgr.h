#pragma once
#include "Engine.h"




class TimerMgr
{
	SINGLE(TimerMgr)

public:
	void Update();
	void AddTimer(Timer* timer);

	void ChangeScene();

private:
	std::vector<Timer*> mTimers;

};

