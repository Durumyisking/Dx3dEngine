#include "GameNetServer.h"
namespace dru::server
{
	GameNetServer::GameNetServer()
	{
	}

	GameNetServer::~GameNetServer()
	{
	}


    // 서버 초기화 및 서버 열기
	void GameNetServer::OpenHost(short port, std::function<void(SOCKET)> accpet, int backLog)
	{
        WSAData WsaData;

        int errorCode = WSAStartup(MAKEWORD(2, 2), &WsaData);
        if (SOCKET_ERROR == errorCode)
        {
            MsgBoxAssert("socket Error");
            return;
        }

        SOCKADDR_IN Add;
        Add.sin_family = AF_INET; // ip4주소 체계를 쓰겠다.
        Add.sin_port = htons(port); // 네트워크 통신에 유효한 에디안 방식으로 만들어준다.
        if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
        {
            return;
        }

        AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (INVALID_SOCKET == AcceptSocket)
        {
            return;
        }

        if (SOCKET_ERROR == bind(AcceptSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
        {
            return;
        }

        // 클라이언트 받기 시작
        if (SOCKET_ERROR == listen(AcceptSocket, backLog))
        {
            return;
        }

        NewUserAccpetFunction = accpet;

        AccpetThread.Start("AccpetThread", std::bind(AcceptThread, AcceptSocket, this));
    }

	void GameNetServer::Send(void* data, unsigned int size)
	{
	}

	void GameNetServer::PacketSend(std::shared_ptr<GameNetPacket> packet)
	{
	}

	void GameNetServer::AcceptThread(SOCKET acceptSocket, GameNetServer* net)
	{
        if (nullptr == net->NewUserAccpetFunction)
        {
            MsgBoxAssert("접속자 처리 로직을 입력해주지 않았습니다.");
            return;
        }

        int AddressLen = sizeof(SOCKADDR_IN);
        while (true == net->GetIsNet())
        {
            SOCKADDR_IN ClientAdd;

            memset(&ClientAdd, 0, sizeof(ClientAdd));

            SOCKET CientSocket = accept(acceptSocket, (sockaddr*)&ClientAdd, &AddressLen);

            if (SOCKET_ERROR == CientSocket || INVALID_SOCKET == CientSocket)
            {
                return;
            }

            if (false == net->GetIsNet())
            {
                return;
            }

            net->UserLock.lock();
            std::shared_ptr<GameNetThread> Thread = std::make_shared<GameNetThread>();
            net->RecvThreads.push_back(Thread);
            net->Users.push_back(CientSocket);

            net->AcceptLock.lock();
            net->NewUserWorks.push(
                [=]()
            {
                net->NewUserAccpetFunction(CientSocket);
            }
            );
            net->AcceptLock.unlock();

            std::string ThreadName = std::to_string(CientSocket);
            ThreadName += "ServerRecvThread";
            Thread->Start(ThreadName, std::bind(RecvThreadFunction, CientSocket, net));
            net->UserLock.unlock();
        }
    }


}