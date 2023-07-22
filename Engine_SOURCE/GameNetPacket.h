#pragma once
#include "ServerHeader.h"
#include "GameNetSerializer.h"
#include "StringFunctions.h"

#define	GameNetPacketError -1

namespace server
{
	class GameNetPacket : public std::enable_shared_from_this<GameNetPacket>
	{
	public:
		// constrcuter destructer
		GameNetPacket();
		~GameNetPacket();

		// delete Function
		//GameNetPacket(const GameNetPacket& _Other) = delete;
		//GameNetPacket(GameNetPacket&& _Other) noexcept = delete;
		//GameNetPacket& operator=(const GameNetPacket& _Other) = delete;
		//GameNetPacket& operator=(GameNetPacket&& _Other) noexcept = delete;


		template<typename ConvertType>
		std::shared_ptr<ConvertType> DynamicConvert()
		{			
			//return std::static_pointer_cast<ConvertType>(shared_from_this());
			return std::dynamic_pointer_cast<ConvertType>(shared_from_this()); // this�����͸� sharedptr�� ����ϴ�.
		}


		unsigned int GetPacketID()
		{
			return mPacketID;
		}

		template<typename EnumType>
		EnumType GetPacketIDToEnum()
		{
			return static_cast<EnumType>(mPacketID);
		}

		template<typename EnumType>
		void SetPacketID(EnumType type)
		{
			mPacketID = static_cast<int>(type);
		}

		void SetObjectID(unsigned int object)
		{
			mObjectID = object;
		}

		unsigned int GetObjectID()
		{
			return mObjectID;
		}


		SOCKET GetMaster()
		{
			return mMaster;
		}

		void SetMaster(SOCKET master)
		{
			if (mMaster != -1)
			{
				return;
			}

			mMaster = master;
		}


		void SerializePacket(GameNetSerializer& ser)
		{
			Serialize(ser);
			SerializeEnd(ser);
		}

		void DeSerializePacket(GameNetSerializer& ser)
		{
			DeSerialize(ser);
		}

	protected:

		virtual void Serialize(GameNetSerializer& ser) = 0
		{
			ser << mPacketID;
			ser << mSize;
			ser << mObjectID;
			//ser << mMaster;
		}
		virtual void DeSerialize(GameNetSerializer& ser) = 0
		{
			ser >> mPacketID;
			ser >> mSize;
			ser >> mObjectID;
			//ser >> mMaster;
		}



	private:
		unsigned int mPacketID = -1; // ��Ŷ�� ����
		unsigned int mSize = -1; // ��Ŷ�� ũ��
		unsigned int mObjectID = -1; // ���� ������Ʈ�� ����
		SOCKET mMaster = 0;

		void SerializeEnd(GameNetSerializer& ser)
		{
			mSize = ser.GetWriteOffSet();

			if (mSize <= 0)
			{
				MsgBoxAssert("0����Ʈ ��Ŷ�� �����Ǿ����ϴ�.");
			}

			unsigned char* Ptr = ser.GetDataPtr();
			memcpy_s(&Ptr[4], sizeof(int), &mSize, sizeof(int));
		}
	};

}