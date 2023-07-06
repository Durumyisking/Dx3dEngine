#pragma once
#include "guiWidget.h"
#include "Resource.h"


namespace gui
{
	class Resource : public Widget
	{
	public:
		Resource();
		~Resource();

		virtual void FixedUpdate() override;

		void SetTarget(dru::Resource* target) { mTarget = target; }
		dru::Resource* GetTarget() { return mTarget; }

	private:
		dru::Resource* mTarget;
	};
}
