#pragma once
#include "GameNetPacket.h"
#include "ServerHeader.h"
#include "druMath.h"

namespace dru::server
{
	class ObjectUpdatePacket : public GameNetPacket
	{
	public:
		int ObjectType = -1;
		dru::math::Vector3 Pos;

		ObjectUpdatePacket()
			//: GameNetPacket(static_cast<int>(PacketEnumType::UserIDPacket))
		{
			SetPacketID(PacketEnumType::ObjectUpdatePacket);
		}

	public:
		virtual void Serialize(GameNetSerializer& ser) override
		{
			GameNetPacket::Serialize(ser);
			ser << ObjectType;
			ser.Write(&Pos, sizeof(Pos));
		}
		virtual void DeSerialize(GameNetSerializer& ser) override
		{
			GameNetPacket::DeSerialize(ser);
			ser >> ObjectType;
			ser.Read(&Pos, sizeof(Pos));
		}

	};
}
