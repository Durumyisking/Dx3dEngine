#pragma once
#include "GameObj.h"

namespace dru
{
	class DebugObject :
		public GameObj
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		virtual void fixedUpdate();

	};

}
