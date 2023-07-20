#pragma once
#include "GameObj.h"


namespace gui
{
	class DebugObject : public GameObj
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		virtual void FixedUpdate() override;
	
	};
}
