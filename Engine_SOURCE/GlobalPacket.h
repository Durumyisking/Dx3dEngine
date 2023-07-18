#pragma once
#include "ServerHeader.h"
#include "druMath.h"

namespace dru::server
{
	class ObjectUpdatePacket : public GameNetPacket
	{
	public:
		dru::math::Vector3 Pos;

		ObjectUpdatePacket()
			//: GameNetPacket(static_cast<int>(PacketEnumType::UserIDPacket))
		{
			SetPacketID(PacketEnumType::ObjectUpdatePacket);
		}

	public:
		void Serialize(GameNetSerializer& ser) override
		{
			GameNetPacket::Serialize(ser);

			ser.Write(&Pos, sizeof(Pos));
		}
		void DeSerialize(GameNetSerializer& ser) override
		{
			GameNetPacket::DeSerialize(ser);

			ser.Read(&Pos, sizeof(Pos));
		}

	};
}
