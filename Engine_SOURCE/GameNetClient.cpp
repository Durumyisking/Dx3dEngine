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
        // �� ���α׷��� ��Ʈ��ũ�� ����ҷ���.
        WSAData WsaData;

        // �� ���α׷��� �����쿡�� ������ ����ϰڴٰ� �˷��ִ� �̴ϴ�.
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

        // ����ȣ��Ʈ ip ����ǻ�Ϳ� ���� �����ϰڴ�.
        std::string IP = ip;

        if (SOCKET_ERROR == inet_pton(AF_INET, IP.c_str(), &ClientAdd.sin_addr))
        {
            return;
        }

        int Len = sizeof(SOCKADDR_IN);

        // ���� �ð��� �ʰ��߽��ϴ�.
        if (SOCKET_ERROR == connect(mClientSocket, (const sockaddr*)&ClientAdd, Len))
        {
            MsgBoxAssert("Ŀ��Ʈ�� �����߽��ϴ�.");
            return;
        }

        // ������ ���� �Ǵ� ���� Recv������ �۵�   //bind      �Լ� �̸�          ��     ��     ��
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