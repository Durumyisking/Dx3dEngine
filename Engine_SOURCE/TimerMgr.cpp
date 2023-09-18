#include "TimerMgr.h"
#include "TimeMgr.h"



TimerMgr::TimerMgr()
	: mTimers{}
{

}

TimerMgr::~TimerMgr()
{
	for (Timer* timer : mTimers)
	{
		delete timer;
		timer = nullptr;
	}
	mTimers.clear();
}

void TimerMgr::Update()
{
	for (std::vector<Timer*>::iterator iter = mTimers.begin(); iter != mTimers.end(); )
	{
		if ((*iter)->IsFinished())
		{
			if ((*iter)->IsDestroy())
			{
				delete (*iter);
				(*iter) = nullptr;
				iter = mTimers.erase(iter);
			}
			else
			{
				(*iter)->Restart();
			}
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

void TimerMgr::ChangeScene()
{
	for (Timer* timer : mTimers)
	{
		delete timer;
		timer = nullptr;
	}
	mTimers.clear();
}
