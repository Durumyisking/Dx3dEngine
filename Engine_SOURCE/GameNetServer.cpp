#include "GameNetServer.h"
namespace dru::server
{
	GameNetServer::GameNetServer()
        : mSendSer{}
        , mAccpetThread{}
        , mAcceptSocket{}
        , mUserLock{}
        , mAllUsers{}
        , mRecvThreads{}
        , mNewUserAccpetFunction{}
        , mAcceptLock{}
        , mNewUserWorks{}
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

        mAcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (INVALID_SOCKET == mAcceptSocket)
        {
            return;
        }

        if (SOCKET_ERROR == bind(mAcceptSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
        {
            return;
        }

        // 클라이언트 받기 시작
        if (SOCKET_ERROR == listen(mAcceptSocket, backLog))
        {
            return;
        }

        mNewUserAccpetFunction = accpet;

        mAccpetThread.Start("AccpetThread", std::bind(AcceptThread, mAcceptSocket, this));
    }

	void GameNetServer::Send(void* data, unsigned int size)
	{
        mUserLock.lock();

        for (size_t i = 0; i < mAllUsers.size(); i++)
        {
            send(mAllUsers[i], reinterpret_cast<const char*>(data), size, 0);
        }

        mUserLock.unlock();
    }

	void GameNetServer::PacketSend(std::shared_ptr<GameNetPacket> packet)
	{
        if (true == mAllUsers.empty())
        {
            return;
        }

        mUserLock.lock();

        GameNetSerializer Ser;
        packet->SerializePacket(Ser);
        for (size_t i = 0; i < mAllUsers.size(); i++)
        {
            // 보낸 당사자에게는 보내면 안되요.

            send(mAllUsers[i], reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet(), 0);
        }
        mUserLock.unlock();
    }


	void GameNetServer::AcceptThread(SOCKET acceptSocket, GameNetServer* net)
	{
        if (nullptr == net->mNewUserAccpetFunction)
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

            net->mUserLock.lock();
            std::shared_ptr<GameNetThread> Thread = std::make_shared<GameNetThread>();
            net->mRecvThreads.push_back(Thread);
            net->mAllUsers.push_back(CientSocket);

            net->mAcceptLock.lock();
            net->mNewUserWorks.push(
                [=]()
            {
                net->mNewUserAccpetFunction(CientSocket);
            }
            );
            net->mAcceptLock.unlock();

            std::string ThreadName = std::to_string(CientSocket);
            ThreadName += "ServerRecvThread";
            Thread->Start(ThreadName, std::bind(RecvThreadFunction, CientSocket, net));
            net->mUserLock.unlock();
        }
    }


}