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
	// �������� �����Դϴ�.
	std::wstring wName = AnsiToUniCodeReturn(name);
	SetThreadDescription(GetCurrentThread(), wName.c_str());
	thread->mWorkFunction();
}