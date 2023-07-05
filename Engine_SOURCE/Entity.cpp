#include "Entity.h"


namespace dru
{

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
}