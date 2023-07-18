#pragma once
#include "GameNetThread.h"
#include "GameNet.h"
#include <queue>

namespace dru::server
{
	class GameNetServer
		: public GameNet
	{
	public:
		// constrcuter destructer
		GameNetServer();
		~GameNetServer();

		// delete Function
		GameNetServer(const GameNetServer& _Other) = delete;
		GameNetServer(GameNetServer&& _Other) noexcept = delete;
		GameNetServer& operator=(const GameNetServer& _Other) = delete;
		GameNetServer& operator=(GameNetServer&& _Other) noexcept = delete;

		void SetNewUserAccpetFunction(std::function<void(SOCKET)> newUserAccpetFunction)
		{
			newUserAccpetFunction = NewUserAccpetFunction;
		}

		void OpenHost(short port, std::function<void(SOCKET)> accpet, int backLog = 512);

	protected:
		void Send(void* data, unsigned int size) override;
		void PacketSend(std::shared_ptr<GameNetPacket> packet) override;

		void PacketProcess() override
		{
			AcceptLock.lock();
			while (0 != NewUserWorks.size())
			{
				std::function<void()> Work = NewUserWorks.front();
				NewUserWorks.pop();
				Work();
			}
			AcceptLock.unlock();

			GameNet::PacketProcess();
		}

	private:
		GameNetSerializer SendSer;
		GameNetThread AccpetThread;
		SOCKET AcceptSocket;

		std::mutex UserLock;
		std::vector<SOCKET> Users;
		std::vector<std::shared_ptr<GameNetThread>> RecvThreads;
		std::function<void(SOCKET)> NewUserAccpetFunction;

		std::mutex AcceptLock;
		std::queue<std::function<void()>> NewUserWorks;

		static void AcceptThread(SOCKET acceptSocket, GameNetServer* net);
	};


}