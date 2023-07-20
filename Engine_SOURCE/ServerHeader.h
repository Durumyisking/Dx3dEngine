#pragma once

// 네트워크
#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더
#include <WinSock2.h>
#include <Windows.h>
#include <list>
#include <vector>
#include <mutex>
#include <memory>

#pragma comment (lib, "ws2_32") // <= 윈도우 서버 사용을 위한 라이브러리

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
