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
		// �������� �����Դϴ�.
		std::wstring wName = AnsiToUniCodeReturn(name);
		SetThreadDescription(GetCurrentThread(), wName.c_str());
		thread->mWorkFunction();
	}
}