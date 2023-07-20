#pragma once
#include "ServerHeader.h"
#include <string>
#include <thread>
#include <functional>

namespace server
{
	class GameNetThread
	{
	public:
		GameNetThread();
		~GameNetThread();

		GameNetThread(const GameNetThread& other)
			: mThread()
			, mWorkFunction(nullptr)
		{

		}

		GameNetThread(GameNetThread&& other) noexcept
			: mThread()
			, mWorkFunction(nullptr)
		{

		}


		// delete Function
		// ��������ڿ� �̵��������� ����� ���´�.
		// GameNetThread(const GameNetThread& _Other) = delete;
		// GameNetThread(GameNetThread&& _Other) noexcept = delete;
		GameNetThread& operator=(const GameNetThread& _Other) = delete; 
		GameNetThread& operator=(GameNetThread&& _Other) noexcept = delete; 

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

		static void GameNetThreadFunction(GameNetThread* thread, std::string name);

	};

}
