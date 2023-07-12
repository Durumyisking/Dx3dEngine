#include "guiDebugObject.h"
#include "Component.h"

namespace gui
{
	DebugObject::DebugObject()
	{

	}
	DebugObject::~DebugObject()
	{

	}

	void DebugObject::fixedUpdate()
	{
		for (dru::Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->FixedUpdate();
		}
	}
}