#pragma once
#include "ServerHeader.h"
#include <string>
#include <thread>
#include <functional>

class GameThread
{
public:
	GameThread();
	~GameThread();

	GameThread(const GameThread& other)
		: mThread()
		, mWorkFunction(nullptr)
	{

	}

	GameThread(GameThread&& other) noexcept
		: mThread()
		, mWorkFunction(nullptr)
	{

	}


	// delete Function
	// 복사생성자와 이동생성자의 사용을 막는다.
	// GameThread(const GameThread& _Other) = delete;
	// GameThread(GameThread&& _Other) noexcept = delete;
	GameThread& operator=(const GameThread& _Other) = delete; 
	GameThread& operator=(GameThread&& _Other) noexcept = delete; 

	// 쓰레드 시작하는 함수
	void Start(std::string threadName, std::function<void()>callBack)
	{
		mWorkFunction = callBack;
		mThread = std::thread(GameNetThreadFunction, this, threadName);
	}

	// 쓰레드 Join
	// 해당 쓰레드의 함수 작업이 끝나기 전까지 다른 쓰레드들을 멈춥니다.
	void Join()
	{
		if (nullptr != mWorkFunction)
		{
			Sleep(1);
			mThread.join(); // mThread 작업 끝나면 아랫줄 실행				
			mWorkFunction = nullptr;
		}
	}


private:
	std::thread mThread;
	std::function<void()> mWorkFunction;

	static void GameNetThreadFunction(GameThread* thread, std::string name);

};

