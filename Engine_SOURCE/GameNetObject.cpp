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
			MsgBoxAssert("������Ʈ id�� �������� ���� ���� ������Ʈ�� �ı��Ǿ����ϴ�.");
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
			MsgBoxAssert("�������� �ʴ� ������Ʈ���� ��Ŷ�� ���½��ϴ�.");
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
			MsgBoxAssert("�̹� �����ϴ� ������ƮID�� �� ����Ϸ��� �߽��ϴ�.");
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
			MsgBoxAssert("�̹� �����ϴ� ������ƮID�� �� ����Ϸ��� �߽��ϴ�.");
			return;
		}

		mAllNetObjects.insert(std::pair<int, GameNetObject*>(mObjectID, this));
		mObjectLock.unlock();
	}


}