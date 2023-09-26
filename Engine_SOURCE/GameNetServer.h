#pragma once
#include "ServerHeader.h"
#include "GameThread.h"
#include "GameNet.h"
#include <queue>

namespace server
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
			newUserAccpetFunction = mNewUserAccpetFunction;
		}

		void OpenHost(short port, std::function<void(SOCKET)> accpet, int backLog = 512);

	protected:
		void Send(void* data, unsigned int size) override;
		void PacketSend(std::shared_ptr<GameNetPacket> packet) override;

		void PacketProcess() override
		{
			mAcceptLock.lock();
			while (0 != mNewUserWorks.size())
			{
				std::function<void()> Work = mNewUserWorks.front();
				mNewUserWorks.pop();
				Work();
			}
			mAcceptLock.unlock();

			GameNet::PacketProcess();
		}

	private:
		GameNetSerializer	mSendSer;
		GameThread		mAccpetThread;
		SOCKET				mAcceptSocket;

		std::mutex									mUserLock;
		std::vector<SOCKET>							mAllUsers;
		std::vector<std::shared_ptr<GameThread>> mRecvThreads;
		std::function<void(SOCKET)>					mNewUserAccpetFunction;

		std::mutex									mAcceptLock;
		std::queue<std::function<void()>>			mNewUserWorks;

		static void AcceptThread(SOCKET acceptSocket, GameNetServer* net);
	};


}