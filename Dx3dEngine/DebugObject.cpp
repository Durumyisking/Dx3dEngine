#include "DebugObject.h"
#include "Component.h"

namespace dru
{
	DebugObject::DebugObject()
	{
	}

	DebugObject::~DebugObject()
	{
	}

	void DebugObject::fixedUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (nullptr == comp)
				continue;
			comp->fixedUpdate();
		}
	}

}