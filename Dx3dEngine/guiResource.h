#pragma once
#include "guiWidget.h"
#include "EngineResource.h"


namespace gui
{
	class GUIResource : public Widget
	{
	public:
		GUIResource();
		~GUIResource();

		virtual void FixedUpdate() override;

		void SetTarget(Resource* target) { mTarget = target; }
		Resource* GetTarget() { return mTarget; }

	private:
		Resource* mTarget;
	};
}
