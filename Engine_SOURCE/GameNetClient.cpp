#include "GameNetClient.h"
namespace server
{
	GameNetClient::GameNetClient()
        : mSendSer{}
        , mClientSocket{}
        , mRecvThread{}

	{
		
	}

    GameNetClient::~GameNetClient()
    {
        if (0 != mClientSocket)
        {
            closesocket(mClientSocket);
        }
    }

	void GameNetClient::Connect(const std::string& ip, short port)
	{
        // 내 프로그램이 네트워크를 사용할래요.
        WSAData WsaData;

        // 이 프로그램이 윈도우에게 서버를 사용하겠다고 알려주는 겁니다.
        int errorCode = WSAStartup(MAKEWORD(2, 2), &WsaData);
        if (SOCKET_ERROR == errorCode)
        {
            MsgBoxAssert("socket Error");
            return;
        }

        mClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (INVALID_SOCKET == mClientSocket)
        {
            return;
        }

        SOCKADDR_IN ClientAdd;

        ClientAdd.sin_family = AF_INET;
        ClientAdd.sin_port = htons(30000);

        // 로컬호스트 ip 내컴퓨터에 내가 접속하겠다.
        std::string IP = ip;

        if (SOCKET_ERROR == inet_pton(AF_INET, IP.c_str(), &ClientAdd.sin_addr))
        {
            return;
        }

        int Len = sizeof(SOCKADDR_IN);

        // 접속 시간을 초과했습니다.
        if (SOCKET_ERROR == connect(mClientSocket, (const sockaddr*)&ClientAdd, Len))
        {
            MsgBoxAssert("커넥트에 실패했습니다.");
            return;
        }

        // 서버에 연결 되는 순간 Recv쓰레드 작동   //bind      함수 이름          인     자     들
        mRecvThread.Start("ClientRecvThread", std::bind(RecvThreadFunction, mClientSocket, this));
    }

	void GameNetClient::Send(void* data, unsigned int size)
	{
        send(mClientSocket, reinterpret_cast<const char*>(data), size, 0);
    }

	void GameNetClient::PacketSend(std::shared_ptr<GameNetPacket> packet)
	{
        GameNetSerializer Ser;
        packet->SerializePacket(Ser);
        send(mClientSocket, reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet(), 0);
    }


}