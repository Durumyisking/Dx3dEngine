#pragma once

// ��Ʈ��ũ
#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h> // inet_pton �Լ��� ����ϱ� ���� ���
#include <WinSock2.h>
#include <Windows.h>
#include <list>
#include <vector>
#include <mutex>
#include <memory>

#pragma comment (lib, "ws2_32") // <= ������ ���� ����� ���� ���̺귯��

#include <iostream>
#include <thread>
#include <assert.h>

namespace dru::server
{
	enum class PacketEnumType
	{
		UserIDPacket,
		ObjectUpdatePacket,
	};
}
