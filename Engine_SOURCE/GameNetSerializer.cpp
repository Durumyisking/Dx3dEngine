#include "GameNetSerializer.h"

namespace server
{

	GameNetSerializer::GameNetSerializer()
		: mWriteOffset(0)
		, mReadOffset(0)

	{
		mData.resize(1024);
	}

	GameNetSerializer::GameNetSerializer(const char* data, unsigned int size)
		: mWriteOffset(0)
		, mReadOffset(0)
	{
		mData.resize(size);
		memcpy_s(&mData[0], size, data, size);
	}

	GameNetSerializer::~GameNetSerializer()
	{
	}


	void GameNetSerializer::Write(const void* data, unsigned int size)
	{
		if (mWriteOffset + size >= mData.size())
		{
			mData.resize(mData.capacity() * 2);
		}

		memcpy_s(&mData[mWriteOffset], size, data, size);
		mWriteOffset += size;
	}

	void GameNetSerializer::Read(void* data, unsigned int size)
	{
		memcpy_s(data, size, &mData[mReadOffset], size);
		mReadOffset += size;
	}

	void GameNetSerializer::ClearReadData()
	{
		memcpy_s(&mData[0], mData.size(), &mData[mReadOffset], mWriteOffset - mReadOffset);
		mWriteOffset -= mReadOffset;
		mReadOffset = 0;
	}

	void GameNetSerializer::operator<<(const std::string& value)
	{
		operator<<(static_cast<unsigned int>(value.size()));
		Write(reinterpret_cast<const void*>(&value[0]), static_cast<unsigned int>(value.size()));
	}

	void GameNetSerializer::operator<<(const int value)
	{
		Write(&value, sizeof(int));
	}

	void GameNetSerializer::operator<<(const unsigned int value)
	{
		Write(&value, sizeof(unsigned int));
	}

	void GameNetSerializer::operator<<(const uint64_t& value)
	{
		Write(&value, sizeof(uint64_t));
	}

	void GameNetSerializer::operator<<(const float value)
	{
		Write(&value, sizeof(float));
	}

	void GameNetSerializer::operator>>(std::string& value)
	{
		int Size;
		operator>>(Size);
		if (0 >= Size)
		{
			return;
		}
		value.resize(Size);
		Read(&value[0], Size);
	}

	void GameNetSerializer::operator>>(int& value)
	{
		Read(&value, sizeof(int));
	}

	void GameNetSerializer::operator>>(unsigned int& value)
	{
		Read(&value, sizeof(unsigned int));
	}

	void GameNetSerializer::operator>>(uint64_t& value)
	{
		Read(&value, sizeof(uint64_t));
	}

	void GameNetSerializer::operator>>(float& value)
	{
		Read(&value, sizeof(float));
	}


}