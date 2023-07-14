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
				iter = mTimers.erase(iter);
			}
			else
			{
				++iter;
			}

			(*iter)->Update(DT);
		}
	}

	//void TimerMgr::AddTimer(Timer* timer, std::function<void> func)
	//{
	//	mTimers.push_back(timer);

	//}

}
