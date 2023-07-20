#pragma once
#include "Engine.h"




class TimerMgr
{
	SINGLE(TimerMgr)

public:
	void Update();

	void AddTimer(Timer* timer);

private:
	std::vector<Timer*> mTimers;

};

