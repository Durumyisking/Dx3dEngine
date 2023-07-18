#include "GameNetObject.h"

namespace dru::server
{
	std::atomic<int> GameNetObject::mAtomicObjectID = 0;
	std::mutex GameNetObject::mObjectLock;
	std::map<int, GameNetObject*> GameNetObject::mAllNetObjects;

	GameNetObject::GameNetObject()
	{
	}

	GameNetObject::~GameNetObject()
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


	void GameNetObject::PushObjectPacket(std::shared_ptr<GameNetPacket> packet)
	{
		if (false == mAllNetObjects.contains(packet->GetObjectId()))
		{
			MsgBoxAssert("존재하지 않는 오브젝트에게 패킷을 보냈습니다.");
			return;
		}

		mAllNetObjects[packet->GetObjectId()]->mPackets.push_back(packet);
	}

	bool GameNetObject::IsNetObject(int object)
	{
		return mAllNetObjects.contains(object);
	}

	void GameNetObject::ServerObjectInit()
	{
		mObjectID = ++mAtomicObjectID;
		mObjectLock.lock();

		if (true == mAllNetObjects.contains(mObjectID))
		{
			MsgBoxAssert("이미 존재하는 오브젝트ID에 또 등록하려고 했습니다.");
			return;
		}

		mAllNetObjects.insert(std::pair<int, GameNetObject*>(mObjectID, this));
		mObjectLock.unlock();
	}

	void GameNetObject::ClientObjectInit(int object)
	{
		mObjectID = object;
		mObjectLock.lock();

		if (true == mAllNetObjects.contains(mObjectID))
		{
			MsgBoxAssert("이미 존재하는 오브젝트ID에 또 등록하려고 했습니다.");
			return;
		}

		mAllNetObjects.insert(std::pair<int, GameNetObject*>(mObjectID, this));
		mObjectLock.unlock();
	}


}