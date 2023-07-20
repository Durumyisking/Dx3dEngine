#pragma once
#include "ServerHeader.h"
#include "GameNetThread.h"
#include "GameNet.h"

namespace dru::server
{
	class GameNetClient
		: public GameNet
	{
	public:
		// constrcuter destructer
		GameNetClient();
		~GameNetClient();

		// delete Function
		GameNetClient(const GameNetClient& other) = delete;
		GameNetClient(GameNetClient&& other) noexcept = delete;
		GameNetClient& operator=(const GameNetClient& other) = delete;
		GameNetClient& operator=(GameNetClient&& other) noexcept = delete;

		void Connect(const std::string& ip, short port);

		void Send(void* data, unsigned int size);

		void PacketSend(std::shared_ptr<GameNetPacket> packet);


	protected:

	private:
		GameNetSerializer	mSendSer;
		SOCKET				mClientSocket;
		GameNetThread		mRecvThread;

	};
}
