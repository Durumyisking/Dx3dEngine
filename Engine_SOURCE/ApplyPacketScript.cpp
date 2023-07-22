#include "ApplyPacketScript.h"
#include "GameObj.h"
#include "GlobalPacket.h"
#include "Physical.h"



using namespace server;

ApplyPacketScript::ApplyPacketScript()
{
}

ApplyPacketScript::~ApplyPacketScript()
{
}

void ApplyPacketScript::Initialize()
{
}

void ApplyPacketScript::Update()
{
	if (0 == mPackets.size())
	{
		return;
	}

	Transform* tr = GetOwner()->GetComponent<Transform>();

	while (0 != mPackets.size())
	{
		std::shared_ptr<GameNetPacket> Packet = mPackets.front();

		PacketEnumType Type = Packet->GetPacketIDToEnum<PacketEnumType>();

		switch (Type)
		{
		case PacketEnumType::ObjectUpdatePacket:
		{
			std::shared_ptr<ObjectUpdatePacket> ObjectPacket = Packet->DynamicConvert<ObjectUpdatePacket>();

			if (eActorType::Dynamic == GetOwner()->GetComponent<Physical>()->GetActorType())
			{
				tr->SetPhysicalPosition(ObjectPacket->Pos);
			}
			else
			{
				tr->SetPosition(ObjectPacket->Pos);
			}
			break;
		}
		default:
			break;
		}

		mPackets.pop_front();
	}
}

void ApplyPacketScript::FixedUpdate()
{
}

void ApplyPacketScript::Render()
{
}

void ApplyPacketScript::fontRender()
{
}

