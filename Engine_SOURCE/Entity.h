#pragma once
#include "Engine.h"


namespace dru
{
	class Entity
	{
	public:
		Entity();
		Entity(const Entity& other);
		virtual ~Entity();

		void SetName(const std::wstring& name) { mName = name; }
		const std::wstring& GetName() { return mName; }
		uintptr_t GetID() { return mID; }

	private:
		std::wstring mName;
		const uintptr_t mID;

	};

}




namespace gui
{
	class Entity
	{
	public:
		Entity();
		Entity(const char* name);
		Entity(const Entity& other);
		virtual ~Entity();

		void SetName(const std::string& name) { mName = name; }
		const std::string& GetName() { return mName; }
		UINT32 GetID() { return mID; }

	private:
		std::string mName;
		const UINT32 mID;

	};
}
