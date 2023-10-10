#include "GameThread.h"
#include "StringFunctions.h"


GameThread::GameThread()
{
}
GameThread::~GameThread()
{
	Join();
}
void GameThread::GameNetThreadFunction(GameThread* thread, std::string name)
{
	// 쓰레드의 영역입니다.
	std::wstring wName = AnsiToUniCodeReturn(name);
	SetThreadDescription(GetCurrentThread(), wName.c_str());
	thread->mWorkFunction();
}