#include "GameNetPacket.h"
namespace dru::server
{
	GameNetPacket::GameNetPacket()
		: mPacketID(GameNetPacketError)
		, mMaster(-1)
	{

	}

	GameNetPacket::~GameNetPacket()
	{

	}

}