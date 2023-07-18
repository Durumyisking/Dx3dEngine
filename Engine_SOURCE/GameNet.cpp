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
			int Result = recv(socket, Data, sizeof(Data), 0); // recv한 데이터의 사이즈가 반환됩니다

			if (-1 == Result)
			{
				return;
			}

			if (SOCKET_ERROR == socket || INVALID_SOCKET == socket)
			{
				return;
			}

			// 읽어온 바이트만큼 시리얼라이즈 mWriteOffset 추가
			Serializer.Write(Data, Result);

			if (8 > Serializer.GetWriteOffSet())
			{
				// ID랑 mSize 다 못읽어왔으면 다시 recv
				continue;
			}

			// 패킷 앞 8바이트 처음 받아왔을때만 들어감
			// 패킷 타입과 패킷 사이즈를 받아온 데이터에서 설정해준다
			if (-1 == PacketSize)
			{
				{
					unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0]; // id 시작주소
					int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
					PacketType = *ConvertPtr;
				}

				{
					unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4]; // size 시작주소
					int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
					PacketSize = *ConvertPtr;
				}
			}

			if (PacketSize > Serializer.GetWriteOffSet())
			{
				// 패킷 아직 다 못읽어왔으면 다시 recv
				continue;
			}

			// 패킷 다 받아왔으면 패킷 객체 생성
			// 읽어온 패킷 
			std::shared_ptr<GameNetPacket> Packet = net->mDispatcher.ConvertPacket(PacketType, Serializer);
			net->PushPacket(Packet);

			// 잘 읽어왔으면 리셋
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

// 질문거리
/*
서버컴퓨터는 접속하면서 플레이어를 생성해 클라처럼 쓰지만
클라이언트의 역할은 전혀 하지 않는건가요?
*/


