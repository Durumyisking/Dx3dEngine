#include "GameNet.h"

namespace dru::server
{
	GameNet::GameNet()
	{
	}

	GameNet::~GameNet()
	{
		mbIsNet = false;
	}

	void GameNet::PacketProcess()
	{	
		mPacketLock.lock();
		mClientRecvPackets = mRecvPackets;
		mRecvPackets.clear();
		mPacketLock.unlock();

		while (mClientRecvPackets.size())
		{
			std::shared_ptr<GameNetPacket> Packet = mClientRecvPackets.front();
			mClientRecvPackets.pop_front();
			mDispatcher.ProcessPacket(Packet);
		}
	}

	void GameNet::RecvThreadFunction(SOCKET socket, GameNet* net)
	{
		char Data[1024] = { 0 };
		GameNetSerializer Serializer;

		int PacketType = -1;
		int PacketSize = -1;


		while (true == net->GetIsNet())
		{
			int Result = recv(socket, Data, sizeof(Data), 0); // recv�� �������� ����� ��ȯ�˴ϴ�

			if (-1 == Result)
			{
				return;
			}

			if (SOCKET_ERROR == socket || INVALID_SOCKET == socket)
			{
				return;
			}

			// �о�� ����Ʈ��ŭ �ø�������� mWriteOffset �߰�
			Serializer.Write(Data, Result);

			if (8 > Serializer.GetWriteOffSet())
			{
				// ID�� mSize �� ���о������ �ٽ� recv
				continue;
			}

			// ��Ŷ �� 8����Ʈ ó�� �޾ƿ������� ��
			// ��Ŷ Ÿ�԰� ��Ŷ ����� �޾ƿ� �����Ϳ��� �������ش�
			if (-1 == PacketSize)
			{
				{
					unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0]; // id �����ּ�
					int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
					PacketType = *ConvertPtr;
				}

				{
					unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4]; // size �����ּ�
					int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
					PacketSize = *ConvertPtr;
				}
			}

			if (PacketSize > Serializer.GetWriteOffSet())
			{
				// ��Ŷ ���� �� ���о������ �ٽ� recv
				continue;
			}

			// ��Ŷ �� �޾ƿ����� ��Ŷ ��ü ����
			// �о�� ��Ŷ 
			std::shared_ptr<GameNetPacket> Packet = net->mDispatcher.ConvertPacket(PacketType, Serializer);
			net->PushPacket(Packet);

			// �� �о������ ����
			if (Serializer.GetWriteOffSet() == Serializer.GetReadOffSet())
			{
				Serializer.Reset();
				PacketType = -1;
				PacketSize = -1;
			}
			// 
			else if (Serializer.GetWriteOffSet() > Serializer.GetReadOffSet())
			{
				Serializer.ClearReadData();
			}
		}
	}

}

// �����Ÿ�
/*
������ǻ�ʹ� �����ϸ鼭 �÷��̾ ������ Ŭ��ó�� ������
Ŭ���̾�Ʈ�� ������ ���� ���� �ʴ°ǰ���?
*/


