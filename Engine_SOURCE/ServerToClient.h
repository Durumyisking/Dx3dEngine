#pragma once
#include "GameNetPacket.h"

namespace server
{
	class UserIDPacket : public GameNetPacket
	{
	public:
		UserIDPacket()
			//: GameNetPacket(static_cast<int>(PacketEnumType::UserIDPacket))
		{
			SetPacketID(PacketEnumType::UserIDPacket);
		}

	public:
		void Serialize(GameNetSerializer& _Ser) override
		{
			GameNetPacket::Serialize(_Ser);
		}
		void DeSerialize(GameNetSerializer& _Ser) override
		{
			GameNetPacket::DeSerialize(_Ser);
		}
	};
}