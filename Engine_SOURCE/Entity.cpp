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

void Entity::Save(FILE* File)
{
	int numWChars = (int)mName.length();

	fwrite(&numWChars, sizeof(int), 1, File);
	fwrite(mName.c_str(), sizeof(wchar_t), numWChars, File);
}

void Entity::Load(FILE* File)
{
	int numWChars = 0;

	fread(&numWChars, sizeof(int), 1, File);

	mName.resize(numWChars);
	fread(&mName[0], sizeof(wchar_t), numWChars, File);
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