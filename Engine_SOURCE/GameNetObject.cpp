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
				MsgBoxAssert("������Ʈ id�� �������� ���� ���� ������Ʈ�� �ı��Ǿ����ϴ�.");
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
				MsgBoxAssert("�������� �ʴ� ������Ʈ���� ��Ŷ�� ���½��ϴ�.");
				return;
			}

			mAllNetObjects[packet->GetObjectID()]->mPackets.push_back(packet);
		}
		else
		{
			MsgBoxAssert("��Ʈ��ũ ���� ���°� �ƴմϴ�.");
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
			MsgBoxAssert("��Ʈ��ũ ���� ���°� �ƴմϴ�.");
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
				MsgBoxAssert("�̹� �����ϴ� ������ƮID�� �� ����Ϸ��� �߽��ϴ�.");
				return;
			}

			mAllNetObjects.insert(std::pair<int, GameNetObject*>(mObjectID, this));
			mObjectLock.unlock();
		}
		else
		{
			MsgBoxAssert("��Ʈ��ũ ���� ���°� �ƴմϴ�.");
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
				MsgBoxAssert("�̹� �����ϴ� ������ƮID�� �� ����Ϸ��� �߽��ϴ�.");
				return;
			}

			mAllNetObjects.insert(std::pair<int, GameNetObject*>(mObjectID, this));
			mObjectLock.unlock();
		}
		else
		{
			MsgBoxAssert("��Ʈ��ũ ���� ���°� �ƴմϴ�.");
			return;
		}
	}
}