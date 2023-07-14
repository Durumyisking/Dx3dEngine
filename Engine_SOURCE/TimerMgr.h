#pragma once
#include "Engine.h"


namespace dru
{
	class TimerMgr
	{
		SINGLE(TimerMgr)

	public:
		void Update();

//		void AddTimer(Timer* timer, std::function<void> func);

	private:
		std::vector<Timer*> mTimers;

	};
}
