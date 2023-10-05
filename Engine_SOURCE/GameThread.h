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
	// ��������ڿ� �̵��������� ����� ���´�.
	// GameThread(const GameThread& _Other) = delete;
	// GameThread(GameThread&& _Other) noexcept = delete;
	GameThread& operator=(const GameThread& _Other) = delete; 
	GameThread& operator=(GameThread&& _Other) noexcept = delete; 

	// ������ �����ϴ� �Լ�
	void Start(std::string threadName, std::function<void()>callBack)
	{
		mWorkFunction = callBack;
		mThread = std::thread(GameNetThreadFunction, this, threadName);
	}

	// ������ Join
	// �ش� �������� �Լ� �۾��� ������ ������ �ٸ� ��������� ����ϴ�.
	void Join()
	{
		if (nullptr != mWorkFunction)
		{
			Sleep(1);
			mThread.join(); // mThread �۾� ������ �Ʒ��� ����				
			mWorkFunction = nullptr;
		}
	}


private:
	std::thread mThread;
	std::function<void()> mWorkFunction;

	static void GameNetThreadFunction(GameThread* thread, std::string name);

};

