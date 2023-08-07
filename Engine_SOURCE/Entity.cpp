#include "Entity.h"



Entity::Entity()
	: mID(reinterpret_cast<uintptr_t>(this))
{
}
Entity::Entity(const Entity& other)
	: mID(reinterpret_cast<uintptr_t>(this))
	, mName(other.mName)
{
}
Entity::~Entity()
{
}


namespace gui
{
	Entity::Entity()
		: mID(reinterpret_cast<uintptr_t>(this))
		, mName("")
	{
	}

	Entity::Entity(const char* name)
		: mID	(reinterpret_cast<uintptr_t>(this))
		, mName(name)
	{
	}

	Entity::Entity(const Entity& other)
		: mID(reinterpret_cast<uintptr_t>(this))
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}