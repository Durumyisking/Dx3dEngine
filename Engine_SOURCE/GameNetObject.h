#pragma once
#include <atomic>
#include <queue>
#include <map>
#include <mutex>
#include "GameNetPacket.h"

namespace dru::server
{
	class GameNetObject
	{
	public:
		GameNetObject();
		virtual ~GameNetObject();

		static void PushObjectPacket(std::shared_ptr<GameNetPacket> packet);
		static bool IsNetObject(int object);

		int GetObjectID()
		{
			return mObjectID;
		}

		void ServerObjectInit();

		void ClientObjectInit(int object);


	protected:
		int mObjectID = -1;
		std::list<std::shared_ptr<GameNetPacket>> mPackets;

	private:
		static std::atomic<int> mAtomicObjectID;
		static std::mutex mObjectLock;
		static std::map<int, GameNetObject*> mAllNetObjects;
	};
}