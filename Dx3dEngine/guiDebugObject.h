#pragma once
#include "GameObj.h"


namespace gui
{
	class DebugObject : public dru::GameObj
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		virtual void FixedUpdate() override;
	
	};
}
