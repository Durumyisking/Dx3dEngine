#include "GameNetPacket.h"
namespace server
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