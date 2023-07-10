#pragma once
#include "..\Engine_SOURCE\GameObj.h"


namespace gui
{
	class EditorObject : public dru::GameObj
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();

	private:
		
	};
}