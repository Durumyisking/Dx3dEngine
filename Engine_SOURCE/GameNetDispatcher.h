#pragma once
#include "GameNetPacket.h"
#include <functional>
#include <map>

namespace dru::server
{
	class GameNetDispatcher
	{
	public:
		// constrcuter destructer
		GameNetDispatcher();
		~GameNetDispatcher();

		// delete Function
		GameNetDispatcher(const GameNetDispatcher& other) = delete;
		GameNetDispatcher(GameNetDispatcher&& other) noexcept = delete;
		GameNetDispatcher& operator=(const GameNetDispatcher& other) = delete;
		GameNetDispatcher& operator=(GameNetDispatcher&& other) noexcept = delete;

		template<typename PacketType, typename EnumType>
		void AddHandler(EnumType type, std::function<void(std::shared_ptr<PacketType>)> callBack)
		{
			AddHandler(static_cast<int>(type), callBack);
		}

		template<typename PacketType>
		void AddHandler(int type, std::function<void(std::shared_ptr<PacketType>)> callBack)
		{
			mReturnFunctionMap[type] = [=](GameNetSerializer& ser)
			{
				std::shared_ptr<GameNetPacket> Pakcet = std::make_shared<PacketType>();
				Pakcet->DeSerializePacket(ser);
				return Pakcet;
			};


			mPacketProcessMap[type] = [=](std::shared_ptr<GameNetPacket> packet)
			{
				std::shared_ptr<PacketType> ConvertPacket = std::dynamic_pointer_cast<PacketType>(packet);

				if (nullptr == ConvertPacket)
				{
					MsgBoxAssert("패킷 컨버트에 실패했습니다.");
				}

				callBack(ConvertPacket);
			};
		}

		std::shared_ptr<GameNetPacket> ConvertPacket(int type, GameNetSerializer& ser)
		{
			if (false == mReturnFunctionMap.contains(type))
			{
				MsgBoxAssert("존재하지 않는 리시브 패킷 타입입니다.");
				return nullptr;
			}

			return mReturnFunctionMap[type](ser);
		}

		void ProcessPacket(std::shared_ptr<GameNetPacket> packet)
		{
			if (false == mPacketProcessMap.contains(packet->GetPacketID()))
			{
				MsgBoxAssert("처리 방식을 결정하지 않은 패킷입니다");
			}

			mPacketProcessMap[packet->GetPacketID()](packet);
		}

	private:
		std::map<int, std::function<std::shared_ptr<GameNetPacket>(GameNetSerializer& ser)>> mReturnFunctionMap;
		std::map<int, std::function<void(std::shared_ptr<GameNetPacket>)>> mPacketProcessMap;
	};

}
