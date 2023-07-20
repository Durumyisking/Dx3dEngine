#include "GameNetObject.h"
#include "ServerMgr.h"

namespace dru::server
{
	std::atomic<int> GameNetObject::mAtomicObjectID = 0;
	std::mutex GameNetObject::mObjectLock;
	std::map<int, GameNetObject*> GameNetObject::mAllNetObjects;

	GameNetObject::GameNetObject()
		: mPackets{}
	{

	}

	GameNetObject::~GameNetObject()
	{
		if (GETSINGLE(ServerMgr)->IsNetworkOn())
		{
			if (mObjectID == -1)
			{
				MsgBoxAssert("오브젝트 id가 지정되지 않은 서버 오브젝트가 파괴되었습니다.");
				return;
			}

			mObjectLock.lock();
			mAllNetObjects.erase(mObjectID);
			mObjectLock.unlock();
		}
	}


	void GameNetObject::PushObjectPacket(std::shared_ptr<GameNetPacket> packet)
	{
		if (GETSINGLE(ServerMgr)->IsNetworkOn())
		{
			if (false == mAllNetObjects.contains(packet->GetObjectID()))
			{
				MsgBoxAssert("존재하지 않는 오브젝트에게 패킷을 보냈습니다.");
				return;
			}

			mAllNetObjects[packet->GetObjectID()]->mPackets.push_back(packet);
		}
		else
		{
			MsgBoxAssert("네트워크 연결 상태가 아닙니다.");
			return;
		}
	}

	bool GameNetObject::IsNetObject(int object)
	{
		if (GETSINGLE(ServerMgr)->IsNetworkOn())
		{
			return mAllNetObjects.contains(object);
		}
		else
		{
			MsgBoxAssert("네트워크 연결 상태가 아닙니다.");
			return false;
		}
	}

	void GameNetObject::ServerObjectInit()
	{
		if (GETSINGLE(ServerMgr)->IsNetworkOn())
		{
			mObjectID = ++mAtomicObjectID;
			mObjectLock.lock();

			if (true == mAllNetObjects.contains(mObjectID))
			{
				mObjectLock.unlock();
				MsgBoxAssert("이미 존재하는 오브젝트ID에 또 등록하려고 했습니다.");
				return;
			}

			mAllNetObjects.insert(std::pair<int, GameNetObject*>(mObjectID, this));
			mObjectLock.unlock();
		}
		else
		{
			MsgBoxAssert("네트워크 연결 상태가 아닙니다.");
			return;
		}
	}

	void GameNetObject::ClientObjectInit(int object)
	{
		if (GETSINGLE(ServerMgr)->IsNetworkOn())
		{

			mObjectID = object;
			mObjectLock.lock();

			if (true == mAllNetObjects.contains(mObjectID))
			{
				mObjectLock.unlock();
				MsgBoxAssert("이미 존재하는 오브젝트ID에 또 등록하려고 했습니다.");
				return;
			}

			mAllNetObjects.insert(std::pair<int, GameNetObject*>(mObjectID, this));
			mObjectLock.unlock();
		}
		else
		{
			MsgBoxAssert("네트워크 연결 상태가 아닙니다.");
			return;
		}
	}
}