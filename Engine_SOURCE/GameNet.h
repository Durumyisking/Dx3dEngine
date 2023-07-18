#pragma once
#include <atomic>

#include "GameNetDispatcher.h"
#include "GameNetPacket.h"

namespace dru::server
{
	class GameNet
	{
	public:
		GameNet();
		virtual ~GameNet();

		bool GetIsNet()
		{
			return mbIsNet;
		}

		virtual void Send(void* data, unsigned int size) = 0;

		virtual void PacketSend(std::shared_ptr<GameNetPacket> packet) = 0;


		void PushPacket(std::shared_ptr<GameNetPacket> packet)
		{
			mPacketLock.lock();
			mRecvPackets.push_back(packet);
			mPacketLock.unlock();
		}

		// 클라이언트에서 사용하는 용도 
		virtual void PacketProcess();

		GameNetDispatcher mDispatcher;

		std::mutex mPacketLock;
		std::list<std::shared_ptr<GameNetPacket>> mRecvPackets;
		std::list<std::shared_ptr<GameNetPacket>> mClientRecvPackets;

	protected:
		std::atomic<bool> mbIsNet = true;
		static void RecvThreadFunction(SOCKET socket, GameNet* net);
	};

}