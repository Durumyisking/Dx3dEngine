#pragma once
#include "ServerHeader.h"

namespace server
{
	class GameNetSerializer
	{
	public:
		// constrcuter destructer
		GameNetSerializer();
		GameNetSerializer(const char* data, unsigned int size);
		~GameNetSerializer();

		//// delete Function
		//GameNetSerializer(const GameNetSerializer& _Other) = delete;
		//GameNetSerializer(GameNetSerializer&& _Other) noexcept = delete;
		//GameNetSerializer& operator=(const GameNetSerializer& _Other) = delete;
		//GameNetSerializer& operator=(GameNetSerializer&& _Other) noexcept = delete;

		int GetReadOffSet()
		{
			return mReadOffset;
		}

		void ReadOffsetReset()
		{
			mReadOffset = 0;
		}

		int GetWriteOffSet()
		{
			return mWriteOffset;
		}

		void WriteOffsetReset()
		{
			mWriteOffset = 0;
		}

		void Reset()
		{
			mWriteOffset = 0;
			mReadOffset = 0;
		}

		const std::vector<unsigned char>& GetData() 
		{
			return mData;
		}

		unsigned char* GetDataPtr()
		{
			return &mData[0];
		}

		const char* GetDataConstPtr()
		{
			return reinterpret_cast<const char*>(&mData[0]);
		}

		template<typename Type>
		Type GetDataPtrConvert() 
		{
			return reinterpret_cast<Type>(&mData[0]);
		}

		void Write(const void* data, unsigned int size);

		void Read(void* data, unsigned int size);

		void ClearReadData();

		void operator<<(const std::string& value);
		void operator<<(const int value);
		void operator<<(const unsigned int value);
		void operator<<(const uint64_t& value);
		void operator<<(const float value);

		template<typename T>
		void WriteEnum(const T value)
		{
			Write(reinterpret_cast<const void*>(&value), static_cast<unsigned int>(sizeof(T)));
		}

		template<typename T>
		void WriteUserData(const T value)
		{
			Write(reinterpret_cast<const void*>(&value), static_cast<unsigned int>(sizeof(T)));
		}


		void operator>>(std::string& value);
		void operator>>(int& value);
		void operator>>(unsigned int& value);
		void operator>>(uint64_t& value);
		void operator>>(float& value);

		template<typename T>
		void ReadEnum(T& value)
		{
			Read(reinterpret_cast<void*>(&value), static_cast<unsigned int>(sizeof(T)));
		}

		template<typename T>
		void ReadUserData(T& value)
		{
			Read(reinterpret_cast<void*>(&value), static_cast<unsigned int>(sizeof(T)));
		}

	protected:

	private:
		int mWriteOffset = 0;
		int mReadOffset = 0;
		std::vector<unsigned char> mData;

	};
}
