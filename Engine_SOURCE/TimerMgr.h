#pragma once
#include "Engine.h"


namespace dru
{
	class TimerMgr
	{
		SINGLE(TimerMgr)

	public:
		void Update();

		void AddTimer(Timer* timer);

	private:
		std::vector<Timer*> mTimers;

	};
}
