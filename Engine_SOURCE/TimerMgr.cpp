#include "TimerMgr.h"
#include "TimeMgr.h"

namespace dru
{
	TimerMgr::TimerMgr()
		: mTimers{}
	{

	}

	TimerMgr::~TimerMgr()
	{

	}

	void TimerMgr::Update()
	{
		for (std::vector<Timer*>::iterator iter = mTimers.begin(); iter != mTimers.end(); )
		{
			if ((*iter)->IsFinished())
			{
				delete (*iter);
				(*iter) = nullptr;
				iter = mTimers.erase(iter);
			}
			else
			{
				(*iter)->Update(DT);
				++iter;
			}
		}
	}

	void TimerMgr::AddTimer(Timer* timer)
	{
		mTimers.push_back(timer);
		timer->Start();
	}
}
