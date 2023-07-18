#include "GameNetThread.h"
#include "StringFunctions.h"

namespace dru::server
{
	GameNetThread::GameNetThread()
	{
	}
	GameNetThread::~GameNetThread()
	{
		Join();
	}
	void GameNetThread::GameNetThreadFunction(GameNetThread* thread, std::string name)
	{
		// 쓰레드의 영역입니다.
		std::wstring wName = AnsiToUniCodeReturn(name);
		SetThreadDescription(GetCurrentThread(), wName.c_str());
		thread->mWorkFunction();
	}
}